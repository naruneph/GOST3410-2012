#include "rand.h"
#include "param.h"

bool Z_256_iszero(uint8_t* t){
	for(int i = 0; i < 32; i++){
		if(t[i] != 0x00){
			return false;
		}
	}
	return true;
}

bool Z_512_iszero(uint8_t* t){
	for(int i = 0; i < 64; i++){
		if(t[i] != 0x00){
			return false;
		}
	}
	return true;
}

bool Z_256_l(uint8_t* t, vector<uint8_t>  q){
	for(int i = 31; i >= 0; i--){
		if(t[i]>q[i]){
			return false;
		}
		if(t[i]<q[i]){
			return true;
		}
	}
	return false;
}

bool Z_512_l(uint8_t* t, vector<uint8_t>  q){
	for(int i = 63; i >= 0; i--){
		if(t[i]>q[i]){
			return false;
		}
		if(t[i]<q[i]){
			return true;
		}
	}
	return false;
}

void gen_priv_key_256(uint8_t *dd, struct paramset *pset){
	uint8_t t[32]; 

	do{
	 	rand_bytes(t, sizeof (t));
	}while(Z_256_iszero(t) || !Z_256_l(t, pset-> q.val()));

	memcpy(dd, t, 32);
}

void gen_priv_key_512(uint8_t *dd, struct paramset *pset){
	uint8_t t[64]; 
	do{
		rand_bytes(t, sizeof (t));
	}while(Z_512_iszero(t) || !Z_256_l(t, pset-> q.val()));
	memcpy(dd, t, 64);
}


enum ERRORS{CANNOT_OPEN_OUTPUT_FILE,
			NO_OUTPUT_FILE,
			WRONG_FLAGS
		   };

void Handle_state(int argc, char** argv, map <string,string> &state){
	if(argc > 3){
			throw WRONG_FLAGS;
	}
	string tmp;
	for(int i = 1; i < argc; i++){
		tmp = string(argv[i]);
		if(tmp == "-s"){
			state["pset"] = "SetA";
		} else {
			state["output_file"] = tmp;
		}
	}
	if(state["output_file"] == ""){
		throw NO_OUTPUT_FILE;
	}	
}



int main(int argc, char** argv){
	try{
		map <string,string> state = { {"pset", "SetC"},
									  {"output_file", ""}
									};

		Handle_state(argc, argv, state);

		FILE *output_file = fopen((state["output_file"]).c_str(), "wb");
		if(output_file == NULL){
			throw CANNOT_OPEN_OUTPUT_FILE;
		}

		vector<uint8_t> dd;

		if(state["pset"] == "SetA"){
			BigInt d(32);
			dd = d.val();
		 	gen_priv_key_256(&dd[0], &SetA);
		} else {
			BigInt d(64);
		 	dd = d.val();
		 	gen_priv_key_512(&dd[0], &SetC);
		}

		reverse(dd.begin(),dd.end()); // теперь в dd[0] старший байт

		fwrite(&dd[0],1,dd.size(), output_file);

		fclose(output_file);

	}catch(ERRORS err){
		switch(err){
			case CANNOT_OPEN_OUTPUT_FILE: cerr<<"CANNOT_OPEN_OUTPUT_FILE"<<endl; break;
			case NO_OUTPUT_FILE: cerr<<"NO_OUTPUT_FILE"<<endl; break;
			case WRONG_FLAGS: cerr<<"WRONG_FLAGS"<<endl; break;
			default:cerr<<"ERROR"<<endl; break;
		}
	}
	return 0;
}