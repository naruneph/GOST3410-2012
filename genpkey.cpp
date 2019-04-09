#include "rand.h"
#include "param.h"

BigInt gen_priv_key(struct paramset *pset, int size){
	vector<uint8_t> t(size);

	BigInt zero = vector<uint8_t> {0x00};
	BigInt d;

	do{
		rand_bytes(&t[0], size);
		d =(BigInt) t;
	}while(d == zero || d >= pset->q);

	return d;

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

		BigInt d;

		if(state["pset"] == "SetA"){
			d = gen_priv_key(&SetA, 32);
		} else {
			d = gen_priv_key(&SetC, 64);
		}

		vector<uint8_t> dd = d.val();
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