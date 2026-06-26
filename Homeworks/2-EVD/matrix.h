/* Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .*/
#pragma once
#include<iostream>
#include<cstdio>
#include<cassert>
#include<cstdio>
#include<iomanip>
#include<cmath>
#include<string>
#include<vector>
#include<initializer_list>
#include<functional>
#include<ranges>

namespace pp{

struct vector {
	std::vector<double> data;

	vector() = default;
	vector(int n) : data(n) {}
	vector(std::initializer_list<double> list) : data(list) {}
	vector(const vector&) = default;
	vector(vector&&) noexcept = default;

	vector& operator=(const vector&) = default;
	vector& operator=(vector&&) noexcept = default;

//	auto n(){return std::views::iota(0,size());}
//	void resize(int n) {data.resize(n);}
	inline int size() const {return data.size();}
	inline double& operator[](int i) {return data[i];}
	inline const double& operator[](int i) const {return data[i];}

	vector& operator+=(const vector& other);
	vector& operator-=(const vector& other);
	vector& operator*=(double c);
	vector& operator/=(double c);
	double norm() const;
	void print(std::string s="") const;
	vector map(std::function<double(double)> f) const;
}; //vector

vector operator+(vector a, const vector& b);
vector operator-(vector a)                 ;
vector operator-(vector a, const vector& b);
vector operator*(vector a, const double c) ;
vector operator*(const double c, vector a) ;
vector operator/(vector a, const double c) ;
bool approx(double x, double y, double acc=1e-6, double eps=1e-6);
bool approx(const vector& a,const vector& b,double acc=1e-6,double eps=1e-6);

struct matrix {
	std::vector<pp::vector> cols;
	matrix()=default;
	matrix(int n,int m) : cols(m, pp::vector(n)) {}
	matrix(const matrix& other)=default;
	matrix(matrix&& other)=default;
	matrix& operator=(const matrix& other)=default;
	matrix& operator=(matrix&& other)=default;
	inline double& operator()(int i, int j){return cols[j][i];}
//	inline double& operator()(int i, int j){return cols[j][i];}
	inline const double& operator()(int i, int j)const{return cols[j][i];}
//	inline const double& operator()(int i, int j)const{return cols[j][i];}
	inline vector& operator[](int i){return cols[i];}
	inline const vector& operator[](int i) const {return cols[i];}
//	void resize(int n, int m);
	inline int size1() const {return cols[0].size();}
	inline int size2() const {return cols.size();}
	void setid();
	matrix transpose() const;
	matrix T() const {return transpose();}
	matrix& operator+=(const matrix& B);
	matrix& operator-=(const matrix& B);
	matrix& operator*=(const double c);
	matrix& operator/=(const double c);
	matrix& operator*=(const matrix&);
	matrix  operator^(int);
	void print(std::string s="") const;
}; //matrix

matrix operator+(matrix A, const matrix& B);
matrix operator-(matrix A, const matrix& B);
matrix operator*(const matrix& A, const matrix& B);
matrix operator*(matrix A, const double c);
matrix operator*(const double c, matrix A);
matrix operator/(matrix A, const double c);
bool approx(const matrix& A, const matrix& B, double acc=1e-6, double eps=1e-6);
vector operator*(const matrix& A, const vector& v);

}//pp
