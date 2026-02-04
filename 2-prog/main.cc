#include<iostream>
#include<cstdio>
#include<cmath>
#include<numbers>
#include"hello.h"
#include"sfuns.h"
double gamma(double x){return std::exp(std::lgamma(x));}
int main(){
	hello();

	//Calculations
	double sqrt2=std::sqrt(2.0);
	std::cout << "sqrt(2) = " << sqrt2 << std::endl;
	double two_fifth=std::pow(2.0, 1.0/5.0);
	std::cout << "2^(1/5) = " << two_fifth << std::endl;
	double e_pi=std::pow(std::numbers::e, std::numbers::pi);
	std::cout << "e^pi = " << e_pi << std::endl;
	double pi_e=std::pow(std::numbers::pi, std::numbers::e);
	std::cout << "pi^e = " << pi_e << std::endl;

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
