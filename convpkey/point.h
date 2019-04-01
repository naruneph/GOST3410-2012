#pragma once
#include "headers.h"
#include "BigInt.h"
#include "param.h"

class point{
	public:
		BigInt first;
		BigInt second;
		struct paramset* pset;

		point(BigInt first = vector<uint8_t> {0x00}, BigInt second = vector<uint8_t> {0x01}, struct paramset* pset = &SetC): first(first), second(second), pset(pset){}

		bool IsBitSet(uint8_t, int) const;

		point operator+=(const point &);
		point operator+(const point &) const;
		point operator*(const BigInt &) const;
};

bool point::IsBitSet(uint8_t num, int bit) const{
	return 1 == ( (num >> bit) & 1);
}

point point::operator+=(const point & a){
	BigInt one =  vector<uint8_t> {0x01};

	BigInt u1u2 = first * a.first;
	BigInt v1v2 = second * a.second;

	BigInt du1u2v1v2 = (pset->d * u1u2 * v1v2);

	BigInt u1v2_plus_u2v1 = (first * a.second + second * a.first);
	BigInt v1v2_minus_eu1u1 = (v1v2 - pset->e * u1u2);

	this->first = (((u1v2_plus_u2v1) * (one + du1u2v1v2).inv_mod(pset->p)) / (pset->p))[1];
	this->second = (((v1v2_minus_eu1u1) * (one - du1u2v1v2).inv_mod(pset->p)) / (pset->p))[1];


	return(*this);
}

point point::operator+(const point & a) const{
	point result(*this);
	result += a;
	return result;
}

point point::operator*(const BigInt & a) const{
	point sum;
	point b = (*this);

	int size = a.size();

	bool init = true;

	for(int i = 0; i < size; i++){

		for(int j = 0; j < 8; j++){

			if(!init){            
				b += b;
			}
	
			if(i == 0 && j==0){
				init = false;
			}

			uint8_t aa = a.val()[i];

			if( IsBitSet( aa, j) ){				
				sum += b;
			}
		}	
	}
	return sum;
}