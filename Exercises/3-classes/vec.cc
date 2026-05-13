/* Hold on Gemini 3 */
#include<iostream>
#include<string>
#include<cmath>
#include"vec.h"

vec& vec::operator+=(const vec& other){
	x+=other.x;
	y+=other.y;
	z+=other.z;
	return (*this); // (*this) ~ python's "self"
}

vec& vec::operator-=(const vec& other){
	x-=other.x;
	y-=other.y;
	z-=other.z;
	return (*this); // (*this) ~ python's "self"
}

vec& vec::operator*=(double s){
	x*=s;
	y*=s;
	z*=s;
	return (*this); // (*this) ~ python's "self"
}

vec& vec::operator/=(double s){
	x/=s;
	y/=s;
	z/=s;
	return (*this); // (*this) ~ python's "self"
}

// for debugging
void vec::print(const std::string& s) const {
	std::cout << s << x << " " << y << " " << z << std::endl;
}

// overload "operator<<"
std::ostream& operator<<(std::ostream& os, const vec& v){
	os << "{ " << v.x << ", " << v.y << ", " << v.z << " } ";
	return os;
}

// Semantics of the modern operator+:
// Traditional: vec operator+(const vec& a, const vec& b)
// 	- Forces a manual copy inside: vec r = a;
// Modern:      vec operator+(vec a, const vec& b)
// 	- Passes 'a' by value. This allowa the compilar to perform "Copy Elision" or use "Move Semantics". If a temporary objeft is passed to 'a', the compiler simply reuses it instead of creating a new copy, making it more efficient in modern C++.
vec operator+(vec a, const vec& b){
	a += b;
	return a;
}

vec operator-(vec a, const vec& b){
	a -=b;
	return a;
}

vec operator-(const vec& v){
	return vec(-v.x, -v.y, -v.z);
}

vec operator*(vec a, double s){
	a *= s;
	return a;
}

vec operator*(double s, vec a){
	a *= s;
	return a;
}

vec operator/(vec a, double s){
	a /= s;
	return a;
	
}

double vec::dot(const vec& v) const {
	return x*v.x + y*v.y + z*v.z;
}

double vec::norm() const {
	return std::sqrt(x*x + y*y + z*z);
}

vec vec::cross(const vec& v) const {
	return vec(
	    y * v.z - z * v.y, 
	    z * v.x - x * v.z, 
	    x * v.y - y * v.x
	);
}

// compare two vec's with absolute precision "acc" and relative precision "eps"
static bool approx_doubles(double a, double b, double acc, double eps){
	if (std::abs(a-b) < acc) return true;
	if(std::abs(a-b) / (std::abs(a) + std::abs(b)) < eps) return true;
	return false;
}

bool approx(const vec&a, const vec& b, double acc, double eps){
	if(!approx_doubles(a.x, b.x, acc, eps)) return false;
	if(!approx_doubles(a.y, b.y, acc, eps)) return false;
	if(!approx_doubles(a.z, b.z, acc, eps)) return false;
	return true;
}


