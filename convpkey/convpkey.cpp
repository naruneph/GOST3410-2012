#include "param.h"
#include "point.h"

point get_pub_key(BigInt & d, struct paramset *pset){
	point P(pset->u, pset->v, pset);
	return P * d;
}

enum ERRORS{CANNOT_OPEN_OUTPUT_FILE,
			NO_PRIVATE_KEY,
			WRONG_FLAGS,
			EMPTY_PRIVATE_KEY,
			CANNOT_OPEN_PRIVATE_KEY,
			WRONG_KEY_SIZE
		   };

void Handle_state(int argc, char** argv, map <string,string> &state){
	if(argc > 4){
			throw WRONG_FLAGS;
	}
	string tmp;
	bool pkey_flag = false;
	for(int i = 1; i < argc; i++){
		tmp = string(argv[i]);
		if(tmp == "-s"){
			state["pset"] = "SetA";
		} else if(!pkey_flag){
			state["private_key"] = tmp;
			pkey_flag = true;
			
		} else if(pkey_flag){
			state["output_file"] = tmp;
		}
	}
	if(state["private_key"] == ""){
		throw NO_PRIVATE_KEY;
	}	

}

uint64_t file_size(FILE *f){ 
   fseek(f, 0, SEEK_END);
   uint64_t size = ftell(f);
   fseek(f, 0, SEEK_SET);
   return size;
}


int main(int argc, char** argv){
	try{
		map <string,string> state = { {"pset", "SetC"},
									  {"private_key", ""},
									  {"output_file", "file.pub"}
									};

		Handle_state(argc, argv, state);

		FILE *output_file = fopen((state["output_file"]).c_str(), "wb");
		if(output_file == NULL){
			throw CANNOT_OPEN_OUTPUT_FILE;
		}

		FILE *private_key = fopen((state["private_key"]).c_str(), "rb");
		if(private_key == NULL){
			throw CANNOT_OPEN_PRIVATE_KEY;
		}

		int sz = file_size(private_key);

		if(sz == 0){
			throw EMPTY_PRIVATE_KEY;
		} else if(((sz != 32) && (state["pset"] == "SetA")) || ((sz != 64) && (state["pset"] == "SetC"))){
			throw WRONG_KEY_SIZE;
		}

		vector<uint8_t> dd(sz);

		fread(&dd[0],1,sz,private_key);

		reverse(dd.begin(),dd.end());//теперь в d[0] младший байт

		BigInt d = dd;

		// Q = dP   P(u,v) - все в координатах Эдвардса т.e. надо d раз сложить P

		point Q;
		if(state["pset"] == "SetA"){
			Q = get_pub_key(d, & SetA);
		} else {
			Q = get_pub_key(d, & SetC);
		}

		vector<uint8_t> q_first = Q.first.val();
		vector<uint8_t> q_second = Q.second.val();

		reverse(q_first.begin(), q_first.end()); // теперь в [0] старший байт
		reverse(q_second.begin(), q_second.end()); // теперь в [0] старший байт


		fwrite(&q_first[0],1,q_first.size(), output_file);

		fwrite(&q_second[0],1,q_second.size(), output_file);		

		
		fclose(output_file);
		fclose(private_key);

	}catch(ERRORS err){
		switch(err){
			case EMPTY_PRIVATE_KEY: cerr<<"EMPTY_PRIVATE_KEY"<<endl; break;
			case CANNOT_OPEN_OUTPUT_FILE: cerr<<"CANNOT_OPEN_OUTPUT_FILE"<<endl; break;
			case CANNOT_OPEN_PRIVATE_KEY: cerr<<"CANNOT_OPEN_PRIVATE_KEY"<<endl; break;
			case NO_PRIVATE_KEY: cerr<<"NO_PRIVATE_KEY"<<endl; break;
			case WRONG_FLAGS: cerr<<"WRONG_FLAGS"<<endl; break;
			case WRONG_KEY_SIZE: cerr<<"WRONG_KEY_SIZE"<<endl; break;
			default:cerr<<"ERROR"<<endl; break;
		}
	}
	return 0;
}