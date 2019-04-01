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

		BigInt & operator +=(const BigInt &);
		BigInt & operator -=(const BigInt &);


		BigInt operator+(const BigInt &) const;
		BigInt operator-(const BigInt &) const;
		BigInt operator*(const BigInt &) const;
		vector<BigInt> operator/(const BigInt &) const;
//		BigInt operator%(const BigInt &) const;

		BigInt gcd(BigInt, BigInt, BigInt &, BigInt &);
		BigInt inv_mod(BigInt &);

		void shift_r();
};

void BigInt::remove_leading_zeroes(){
	while(value.size() > 1 && value.back() == 0x00){
		value.pop_back();
	}
	//ноль должен быть положительный
	if(value.size() == 1 && value[0] == 0x00){
		positive = true;
	}
}

BigInt & BigInt::operator=(const BigInt &num){
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
		if(a.value[i] != b.value[i]){
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

BigInt & BigInt::operator +=(const BigInt &num){
	return *this = (*this + num);
}

BigInt & BigInt::operator -=(const BigInt &num){
	return *this = (*this - num);
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
	}
	if(!a.positive){
		return -(-a + b);
	}
	if(a < b){
		return -(b - a);
	}//теперь случай: уменьшаемое больше вычитаемого и все положительное
	uint8_t carry = 0x00;
	for(unsigned  int i = 0; i <  b.value.size() || carry != 0x00; i++){
		int tmp = a.value[i] - carry - (i < b.value.size() ?  b.value[i] : 0x00) ;
		a.value[i] -= (uint8_t) carry + (i < b.value.size() ?  b.value[i] : 0x00);
		carry = (tmp < 0);
		if(carry){
			a.value[i] += 0x100;
		}
	}
	a.remove_leading_zeroes();
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


// BigInt BigInt::operator/(const BigInt &num) const{// деление на ноль??? a/b
// 	BigInt a(*this), b(num);
// 	a.remove_leading_zeroes();
// 	b.remove_leading_zeroes();
// 	a.positive = true;
// 	b.positive = true;

// 	BigInt zero = vector<uint8_t> {0x00};
// 	BigInt one = vector<uint8_t> {0x01};

// 	BigInt count = zero;

// 	while( a >= b){
// 		a -= b;
// 		count += one;
// 	}

// 	count.positive = (this->positive == num.positive);
// 	count.remove_leading_zeroes(); // чтобы 0 был положительным
// 	return count;
// }

void BigInt::shift_r(){
	if(this->value.size() == 0){
		this->value.push_back(0x00);
		
		return;
	}
	this->value.push_back(this->value[this->value.size() - 1]);
	for(int i = this->value.size() - 2; i > 0; i--){
		this->value[i] = this->value[i-1];
	}
	this->value[0] = 0x00;
}

vector<BigInt> BigInt::operator/(const BigInt &num) const{
 	BigInt a(*this), b(num);
 	a.remove_leading_zeroes();
 	b.remove_leading_zeroes();
 	a.positive = true;
	b.positive = true;

	//проверить на деление на ноль

	BigInt res(a.value.size());
	BigInt cur;
	for(int i = a.value.size() - 1; i >= 0; i--){
		cur.shift_r();
		cur.value[0] = a.value[i];
		// подбираем максимальное число x, такое что b * x <= cur
		uint16_t x = 0x00, l = 0x00, r = 0x100;
		while(l <= r){
			uint8_t mm = (l + r) >> 1;    
			BigInt m = vector<uint8_t> {mm};
			BigInt t = b * m;
			if(t <= cur){
				x = mm;  
				l = mm + 0x01;
			} else {
				r = mm - 0x01;
			}
		}
		res.value[i] = (uint8_t) x; 
		cur = cur - b * (BigInt) vector<uint8_t> {(uint8_t) x}; 
	}// есть подозрение что cur это a % b  и похоже это реально так

	// vector<uint8_t> aa = cur.val();
	// for(int i=aa.size() -1; i >=0 ; i--){
	// 	printf("%02x", aa[i]);
	// }

	// cout<<endl;
	// cout<< cur.sgn();

	res.positive = (this->positive == num.positive);
	res.remove_leading_zeroes();
	//return res;
	return vector<BigInt> {res,cur};
}


//BigInt BigInt::operator%(const BigInt &num) const{
	// BigInt a(*this), b(num);
	// a.remove_leading_zeroes();
	// b.remove_leading_zeroes();

	// BigInt res = a - (a/b) * b;
	// if(!res.positive){
	// 	res += b;
	// }
	// return res;
//}

BigInt BigInt::gcd(BigInt a, BigInt b, BigInt &x, BigInt &y){
	BigInt zero = vector<uint8_t> {0x00};
	BigInt one = vector<uint8_t> {0x01};

	if(a == zero){
		x = zero;
		y = one;
		return b;
	}
	BigInt x1, y1;
	vector<BigInt> q = b / a;
	BigInt d = gcd(q[1], a, x1, y1);
	x = y1 - (q[0]) * x1;
	y = x1;
	return d;
}

BigInt BigInt::inv_mod(BigInt & num){
	BigInt one = vector<uint8_t> {0x01};
	BigInt x, y;
	BigInt g = gcd(*this, num, x, y);
	if (g != one){
		throw (string) "CANNOT_GET_INVERSE_ELEMENT";
	}
//	x = (x % num + num) % num;
	x = (x / num)[1];
	return x;
}