#include<iostream>
#include<cstdio>
#include<cmath>
#include<complex>
#include<numbers>
#include"hello.h"
#include"sfuns.h"
using complex=std::complex<double>;
constexpr double  π = 3.14159265358979324;
constexpr double  E = 2.71828182845904523;
constexpr complex I = complex(0,1);

double gamma(double x){return std::exp(std::lgamma(x));}

int main(){
	hello();

	//Calculations
	std::cout << "sqrt(2) = " << std::sqrt(2.0) << "\n";
	std::cout << "2^(1/5) = " << std::pow(2.0, 1.0/5.0) << "\n";
	std::cout << "    E^π = " << std::pow(E, π) << "\n";
    	std::cout << "    E^I = " << std::pow(E,I) <<"\n";
	std::cout << "    π^E = " << std::pow(π, E) << "\n";
    	std::cout << "    π^I = " << std::pow(π,I) <<"\n";
   	std::cout << "    I^I = " << std::pow(I,I) <<"\n";
	std::cout << " log(I) = " << std::log(I)   <<"\n";
    

	//Gamma functions
	double x=1;
	double y = sfuns::fgamma(x);
	std::cout << "fgamma(1)=" << y << "\n";
	for(double x=1;x<=9;x+=1){
		std::cout << "fgamma(" << x << ")=" << sfuns::fgamma(x)
			<< "\t gamma(" << x << ")=" << gamma(x) << "\n";

	}
	std::cout << "gamma(-1)=" << std::lgamma(-1.0) << "\n";
	return 0;
}
