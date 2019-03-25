#pragma once
#include "headers.h"

class BigInt{
	private:
		vector<uint8_t> value;
		bool positive; 
	public:
		BigInt(vector<uint8_t> value = {}, bool positive = true): value(value), positive(positive) {}
		BigInt(int size, bool positive = true){ for(int i = 0; i < size; i++){ value.push_back(0x00); } }


		int size() const{return value.size();}
    	vector<uint8_t> val() const {return value;}
    	bool sgn() const {return positive;}

		void remove_leading_zeroes();

		BigInt & operator =(const BigInt &);

		bool operator==(const BigInt &) const;
		bool operator!=(const BigInt &) const;

		BigInt operator-() const;

		bool operator<(const BigInt &) const;
		bool operator>(const BigInt &) const;
		bool operator<=(const BigInt &) const;
		bool operator>=(const BigInt &) const;

		BigInt operator+(const BigInt &) const;
		BigInt operator-(const BigInt &) const;
		BigInt operator*(const BigInt &) const;
};

void BigInt::remove_leading_zeroes(){
	while(value.size() > 1 && value.back() == 0x00){
		value.pop_back();
	}
	//ноль должен быть положительный
	if(value.size() == 1 && value[0] == 0){
		positive = true;
	}
}

BigInt & BigInt::operator =(const BigInt &num){
	if(this == &num){
		return *this;
	} else {
		value = num.val();
		positive = num.sgn();
		return *this;

	}
}

bool BigInt::operator==(const BigInt &num) const{
	BigInt a(*this), b(num);
	a.remove_leading_zeroes();
	b.remove_leading_zeroes();
	if(a.positive != b.positive){
		return false;
	}
	//раз нет ведущих нулей, то число разрядов должно быть одинаковым
	if(a.value.size() != b.value.size()){
		return false;
	}
	for(unsigned int i = 0; i < a.value.size(); i++){
		if(value[i] != a.value[i]){
			return false;
		}
	}
	return true;
}

bool BigInt::operator!=(const BigInt &num) const{
	return !(*this == num);
}


BigInt BigInt::operator-() const{
		BigInt copy(*this);
		copy.positive = !copy.positive;
		return copy;
}


bool BigInt::operator<(const BigInt &num) const{
	BigInt a(*this), b(num);
	a.remove_leading_zeroes();
	b.remove_leading_zeroes();

	if(a == b){
		return false;
	}
	if(!a.positive){
		if(!b.positive){
			return ((-b) < (-a));
		} else {
			return true;
		}
	} else if(!b.positive){
		return false;
	} else {
		if(a.value.size() != b.value.size()){
			return (a.value.size() < b.value.size());
		} else {
			for(int i = a.value.size()-1; i >= 0; i--){
				if(a.value[i] != b.value[i]){
					return (a.value[i] < b.value[i]);
				}
			}
			return false;
		}
	}	
}

bool BigInt::operator>(const BigInt &num) const{
	return (num < *this);
}

bool BigInt::operator<=(const BigInt &num) const{
	return !(*this > num);
}
bool BigInt::operator>=(const BigInt &num) const{
	return !(*this < num);
}

BigInt BigInt::operator+(const BigInt &num) const{
	BigInt a(*this), b(num);
	a.remove_leading_zeroes();
	b.remove_leading_zeroes();
	if(!a.positive){
		if(!b.positive){
			return -((-a) + (-b));
		} else {
			return b - (-a);
		}
	} else if (!b.positive){
		return a - (-b);
	}//теперь случай: два положительных числа
	uint8_t carry = 0x00;
	for(unsigned int i = 0; i < max(a.value.size(), b.value.size()) || carry != 0x00; i++){
		if(i == a.value.size()){
			a.value.push_back(0x00);
		}
		uint16_t tmp = a.value[i] + carry + (i < b.value.size() ?  b.value[i] : 0x00);
		a.value[i] += (uint8_t) carry + (i < b.value.size() ?  b.value[i] : 0x00);
		carry = tmp >> 8;
	}
	return a;
}

BigInt BigInt::operator-(const BigInt &num) const{
	BigInt a(*this), b(num);
	a.remove_leading_zeroes();
	b.remove_leading_zeroes();
	if(!b.positive){
		return a + (-b);
	} else if(!a.positive){
		return -(-a + b);
	} else if(a < b){
		return -(b - a);
	}//теперь случай: уменьшаемое больше вычитаемого и все положительное
	uint8_t carry = 0x00;
	for(unsigned  int i = 0; i < max(a.value.size(), b.value.size()) || carry != 0x00; i++){
		int8_t tmp = a.value[i] - carry - (i < b.value.size() ?  b.value[i] : 0x00) ;
		a.value[i] -= (uint8_t) carry + (i < b.value.size() ?  b.value[i] : 0x00);
		carry = (tmp < 0);
		if(carry){
			a.value[i] += 0x100;
		}
	}
	return a;
}

BigInt BigInt::operator*(const BigInt &num) const{
	BigInt a(*this), b(num);
	a.remove_leading_zeroes();
	b.remove_leading_zeroes();

	BigInt res;
	for(unsigned int i = 0; i < a.value.size() + b.value.size(); i++){
		res.value.push_back(0x00);
	}
 	res.positive = (a.positive == b.positive);

	for(unsigned int i = 0; i < a.value.size(); i++){
		uint8_t carry = 0;

		for(unsigned int j = 0; j < b.value.size() || carry != 0x00; j++){
			uint16_t tmp = res.value[i+j] + a.value[i] *(j < b.value.size() ?  b.value[j] : 0x00) + carry;
			res.value[i+j] = tmp & 0xff;
			carry = (tmp >> 8);
		}
	}

	res.remove_leading_zeroes();
	return res;
}

