#include "hash.h" //содержит headers
#include "rand.h"
#include "point.h" //содержит BigInt param headers


point Edwards_to_Weierstrass(point CC, struct paramset *pset){
	BigInt one = (BigInt) vector<uint8_t> {0x01};
	BigInt four = (BigInt) vector<uint8_t> {0x04};
	BigInt six = (BigInt) vector<uint8_t> {0x06};
	BigInt inv_four = four.inv_mod(pset->p);
	BigInt inv_six = six.inv_mod(pset->p);

	BigInt S_tmp = ((pset->e + (pset->p - pset->d))/pset->p)[1];
	BigInt S = ((S_tmp * inv_four)/pset->p)[1];

	BigInt T_tmp = ((pset->e + pset->d)/pset->p)[1];
	BigInt T = ((T_tmp * inv_six)/pset->p)[1];

	BigInt numerator = ((S * (((one + CC.second) / (pset->p))[1])  ) / (pset->p))[1];
	BigInt denominator_1 = ((one + ((pset->p - CC.second) / (pset->p))[1] ) / (pset->p))[1];
	BigInt denominator_2 = ((denominator_1 * CC.first)  / (pset->p))[1];

    BigInt X = (((  ((numerator * denominator_1.inv_mod(pset->p)) / (pset->p))[1]   ) + T)  / (pset->p))[1];
	BigInt Y = ((numerator * denominator_2.inv_mod(pset->p))  / (pset->p))[1];

	return point(X,Y, pset);
}


BigInt get_k(struct paramset *pset, int size){
	vector<uint8_t> t(size);
	BigInt zero = vector<uint8_t> {0x00};
	BigInt k;
	do{
		rand_bytes(&t[0], size);
		k =(BigInt) t;
	}while(k == zero || k >= pset->q);
	return k;
}

enum ERRORS{WRONG_FLAGS,
			NO_PRIVATE_KEY_FILE,
			NO_DATA_FILE,
			CANNOT_OPEN_PRIVATE_KEY_FILE,
			CANNOT_OPEN_DATA_FILE,
			EMPTY_PRIVATE_KEY_FILE,
			WRONG_KEY_SIZE,
			CANNOT_OPEN_OUTPUT_FILE
		   };

void Handle_state(int argc, char** argv, map <string,string> &state){
	if(argc > 5){
		throw WRONG_FLAGS;
	}
	string tmp;
	int flag = 0;
	for(int i = 1; i < argc; i++){
		tmp = string(argv[i]);
		if(tmp == "-s"){
			state["pset"] = "SetA";
		} else if(flag == 0){
			state["private_key"] = tmp;
			flag++;
		} else if(flag == 1){
			state["data_file"] = tmp;
			state["output_file"] = tmp + ".crt";
			flag++;
		} else if(flag == 2){
			state["output_file"] = tmp;
		}
	}
	if(state["private_key"] == ""){
		throw NO_PRIVATE_KEY_FILE;
	}
	if(state["data_file"] == ""){
		throw NO_DATA_FILE;
	}
}

uint64_t file_size(FILE *f){ 
   fseek(f, 0, SEEK_END);
   uint64_t size = ftell(f);
   fseek(f, 0, SEEK_SET);
   return size;
}

struct context{
	BigInt e;
	BigInt k;
	BigInt r;
	BigInt s;
	point P;
	struct paramset *pset;

	SHA512::context ctx512;
	SHA256::context ctx256;

	context(struct paramset *pset):P(pset->u, pset->v, pset),  pset(pset){};
	void update(uint8_t *m, int64_t size, string s);
	void sign(uint8_t *sgn, BigInt &d,string sss);
};

void context::update(uint8_t *m, int64_t size, string s){
	(s == "SetC") ? ctx512.update(m, size) : ctx256.update(m, size);
}

void context::sign(uint8_t *sgn, BigInt &d, string sss){
	//считаем хэш от сообщения	
	vector<uint8_t> hash(d.size());
	(sss == "SetC") ? ctx512.finish(hash): ctx256.finish(hash);

	//теперь хэш лежит в hash, для дальнейшей работы сделаем из него BigInt
	BigInt h = hash;

	e = (h / pset->q)[1];
	if(e == (BigInt) vector<uint8_t> {0x00}){
		e = vector<uint8_t> {0x01};
	}

	do{
		do{
			k = get_k(pset, d.size());
			point CC = P * k;
			CC = Edwards_to_Weierstrass(CC, pset);
			r = (CC.first / pset->q)[1];
		}while(r == (BigInt) vector<uint8_t> {0x00});

		s = ((r*d + k*e) / pset->q)[1];

	}while(s == (BigInt) vector<uint8_t> {0x00});

	//sgn = r || s причем от старших к младшим байтам и размер надо подогнать

	r.remove_leading_zeroes();
	s.remove_leading_zeroes();

	r.fix_size(d.size());
	s.fix_size(d.size());

	vector<uint8_t> rr = r.val();
	vector<uint8_t> ss = s.val();

	reverse(rr.begin(), rr.end());//теперь в [0] старший байт
	reverse(ss.begin(), ss.end());

	memcpy(sgn, &rr[0], rr.size());
	memcpy(sgn + rr.size(), &ss[0], ss.size());
}


int ret;

int main(int argc, char** argv){
	try{
		map <string,string> state = { {"pset", "SetC"},
									  {"private_key", ""},
									  {"data_file", ""},
									  {"output_file", ""}
									};

		Handle_state(argc, argv, state);

		FILE *private_key = fopen((state["private_key"]).c_str(), "rb");
		if(private_key == NULL){
			throw CANNOT_OPEN_PRIVATE_KEY_FILE;
		}	

		FILE *data_file = fopen((state["data_file"]).c_str(), "rb");
		if(data_file == NULL){
			throw CANNOT_OPEN_DATA_FILE;
		}

		FILE *output_file = fopen((state["output_file"]).c_str(), "wb");
		if(output_file == NULL){
			throw CANNOT_OPEN_OUTPUT_FILE;
		}

		int sz = file_size(private_key);

		if(sz == 0){
			throw EMPTY_PRIVATE_KEY_FILE;
		} else if(((sz != 32) && (state["pset"] == "SetA")) || ((sz != 64) && (state["pset"] == "SetC"))){
			throw WRONG_KEY_SIZE;
		}

		//получим приватный ключ d
		vector<uint8_t> dd(sz);
		ret = fread(&dd[0],1,sz,private_key);
		reverse(dd.begin(),dd.end());//теперь в d[0] младший байт
		BigInt d = dd;

		context ctx(& SetC);
		if(state["pset"] == "SetA"){
			ctx.pset = & SetA;
			ctx.P.first = SetA.u;
			ctx.P.second = SetA.v;
			ctx.P.pset = & SetA;
		}


		//cчитаем сообщение
		vector<uint8_t> m;
		Set_zero(m);
		int64_t cur_size = fread(&m[0],1,64,data_file);
		do{	
			ctx.update(&m[0], cur_size, state["pset"]);
			cur_size = fread(&m[0], 1, 64, data_file);
		}while(cur_size > 0);

		//теперь получим подпись
		vector<uint8_t> sgn(2*d.size());
		ctx.sign(&sgn[0], d, state["pset"]);


		fwrite(&sgn[0], 1, sgn.size(), output_file);


		fclose(private_key);
		fclose(data_file);
		fclose(output_file);

	} catch(ERRORS err){
		switch(err){
			case WRONG_FLAGS: cerr<<"WRONG_FLAGS"<<endl; break;
			case NO_PRIVATE_KEY_FILE: cerr<<"NO_PRIVATE_KEY_FILE"<<endl; break;
			case NO_DATA_FILE: cerr<<"NO_DATA_FILE"<<endl; break;
			case CANNOT_OPEN_PRIVATE_KEY_FILE: cerr<<"CANNOT_OPEN_PRIVATE_KEY_FILE"<<endl; break;
			case CANNOT_OPEN_DATA_FILE: cerr<<"CANNOT_OPEN_DATA_FILE"<<endl; break;
			case EMPTY_PRIVATE_KEY_FILE: cerr<<"EMPTY_PRIVATE_KEY_FILE"<<endl; break;
			case WRONG_KEY_SIZE: cerr<<"WRONG_KEY_SIZE"<<endl; break;
			case CANNOT_OPEN_OUTPUT_FILE: cerr<<"CANNOT_OPEN_OUTPUT_FILE"<<endl; break;

			default:cerr<<"ERROR"<<endl; break;
		}
	}
	return 0;
}

