#pragma once
#include<string>
#include<iostream>
#include<cstdio>

struct vec {
	double x,y,z;
	//ctors
	vec(double a,double b,double c){ // parm. ctor
		std::cout << "vec: parametrized constructor called...\n";
		x=a;y=b;z=c;	
		}
	vec() : vec(0,0,0) { // default ctor
		std::cout << "vec: default constructor called...\n";
		}
	vec(const vec&)=default; // copy ctor: vec a(b);
	vec(vec&&)=default; // move ctor:  vec a(b+c);
	
	//dtor
	~vec(){ std::cout << "vec: destructor called...\n"; }

	// assignments
	vec& operator=(const vec&)=default; // copy assign. : a=b;
	vec& operator=(vec&&)=default; // move assign. : a=b+c;
	
	//member operators
	vec& operator+=(const vec&);
	vec& operator-=(const vec&);
	vec& operator*=(double);
	vec& operator/=(double);

	// utility
	void set(double a, double b, double c) { x = a; y = b; z = c; }
	double dot(const vec&) const;
	vec cross(const vec&) const;
	double norm() const;
	// for debugging
	void print(const std::string& s = "") const;

	// stream output
	friend std::ostream& operator<<(std::ostream&, const vec&);
};
// non-members
vec operator-(const vec&);
vec operator+(vec, const vec&);
vec operator-(vec, const vec&);
vec operator*(vec, double);
vec operator*(double, vec);
vec operator/(vec, double);

// aproximate equality
bool approx(double a, double b, double acc = 1e-6, double eps = 1e-6);
bool approx(const vec& a, const vec& b, double acc = 1e-6, double eps = 1e-6);

