#include "hash.h" //содержит headers
#include "rand.h"
#include "point.h" //содержит BigInt param headers


enum ERRORS{WRONG_FLAGS,

			NO_PUBLIC_KEY_FILE,
			NO_DATA_FILE,
			NO_CRT_FILE,

			CANNOT_OPEN_PUBLIC_KEY_FILE,
			CANNOT_OPEN_DATA_FILE,
			CANNOT_OPEN_CRT_FILE,

			EMPTY_PUBLIC_KEY_FILE,
			EMPTY_CRT_FILE,

			WRONG_KEY_SIZE
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
			state["public_key"] = tmp;
			flag++;
		} else if(flag == 1){
			state["data_file"] = tmp;
			flag++;
		} else if(flag == 2){
			state["crt_file"] = tmp;
		}
	}
	if(state["public_key"] == ""){
		throw NO_PUBLIC_KEY_FILE;
	}
	if(state["data_file"] == ""){
		throw NO_DATA_FILE;
	}
	if(state["crt_file"] == ""){
		throw NO_CRT_FILE;
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
	bool verify(point & Q, BigInt & r, BigInt & s, string sss);
};

void context::update(uint8_t *m, int64_t size, string s){
	(s == "SetC") ? ctx512.update(m, size) : ctx256.update(m, size);
}

bool context::verify(point & Q, BigInt & r, BigInt & s, string sss){
	BigInt zero = vector<uint8_t> {0x00};
	if(!((zero < r && r < pset->q) && (zero < s && s < pset->q))){
		return false;
	}

	//считаем хэш от сообщения	
	vector<uint8_t> hash(r.size());
	(sss == "SetC") ? ctx512.finish(hash): ctx256.finish(hash);
	//теперь хэш лежит в hash, для дальнейшей работы сделаем из него BigInt
	BigInt h = hash;

	e = (h / pset->q)[1];
	if(e == (BigInt) vector<uint8_t> {0x00}){
		e = vector<uint8_t> {0x01};
	}

	BigInt v = e.inv_mod(pset->q);

	BigInt z1 = ((s * v) / pset->q)[1];
	BigInt z2 = ((-(r * v)) / pset->q)[1];

	point CC = P * z1 + Q * z2;

	BigInt R = (CC.first / pset->q)[1];

	return (R == r);
}


int main(int argc, char** argv){
	try{
		map <string,string> state = { {"pset", "SetC"},
									  {"public_key", ""},
									  {"data_file", ""},
									  {"crt_file", ""}
									};

		Handle_state(argc, argv, state);

		FILE *public_key = fopen((state["public_key"]).c_str(), "rb");
		if(public_key == NULL){
			throw CANNOT_OPEN_PUBLIC_KEY_FILE;
		}	

		FILE *data_file = fopen((state["data_file"]).c_str(), "rb");
		if(data_file == NULL){
			throw CANNOT_OPEN_DATA_FILE;
		}

		FILE *crt_file = fopen((state["crt_file"]).c_str(), "rb");
		if(crt_file == NULL){
			throw CANNOT_OPEN_CRT_FILE;
		}

		int sz1 = file_size(public_key);
		if(sz1 == 0){
			throw EMPTY_PUBLIC_KEY_FILE;
		} else if(((sz1 != 32*2) && (state["pset"] == "SetA")) || ((sz1 != 64*2) && (state["pset"] == "SetC"))){
			throw WRONG_KEY_SIZE;
		}

		int sz2 = file_size(crt_file);
		if(sz2 == 0){
			throw EMPTY_CRT_FILE;
		}

		//получим публичный ключ Q
		vector<uint8_t> q_first(sz1/2), q_second(sz1/2);
		fread(&q_first[0],1,sz1/2,public_key);
		fread(&q_second[0],1,sz1/2,public_key);

		reverse(q_first.begin(), q_first.end());
		reverse(q_second.begin(), q_second.end());

		point Q;
		Q.first = q_first;
		Q.second = q_second;
		if(state["pset"] == "SetA"){
			Q.pset = & SetA;
		}

		//получим подпись - пару r и s
		vector<uint8_t> r(sz2/2), s(sz2/2);
		fread(&r[0],1,sz2/2,crt_file);
		fread(&s[0],1,sz2/2,crt_file);

		reverse(r.begin(), r.end());
		reverse(s.begin(), s.end());

		BigInt R = r;
		BigInt S = s;


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

		//теперь проверим подпись
		bool result = ctx.verify(Q, R, S, state["pset"]);

		if(result){
			cout << "Valid" << endl;
		} else {
			cout << "Invalid" << endl;
		}

		fclose(public_key);
		fclose(data_file);
		fclose(crt_file);

	} catch(ERRORS err){
		switch(err){
			case WRONG_FLAGS: cerr<<"WRONG_FLAGS"<<endl; break;
			case NO_PUBLIC_KEY_FILE: cerr<<"NO_PUBLIC_KEY_FILE"<<endl; break;
			case NO_DATA_FILE: cerr<<"NO_DATA_FILE"<<endl; break;
			case NO_CRT_FILE: cerr<<"NO_CRT_FILE"<<endl; break;
			case CANNOT_OPEN_PUBLIC_KEY_FILE: cerr<<"CANNOT_OPEN_PUBLIC_KEY_FILE"<<endl; break;
			case CANNOT_OPEN_DATA_FILE: cerr<<"CANNOT_OPEN_DATA_FILE"<<endl; break;
			case CANNOT_OPEN_CRT_FILE: cerr<<"CANNOT_OPEN_CRT_FILE"<<endl; break;
			case EMPTY_PUBLIC_KEY_FILE: cerr<<"EMPTY_PUBLIC_KEY_FILE"<<endl; break;
			case EMPTY_CRT_FILE: cerr<<"EMPTY_CRT_FILE"<<endl; break;
			case WRONG_KEY_SIZE: cerr<<"WRONG_KEY_SIZE"<<endl; break;
			default:cerr<<"ERROR"<<endl; break;
		}
	}
	return 0;
}