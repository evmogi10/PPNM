#include<iostream>
#include<cstdio>
#include"hello.h"
#include"sfuns.h"
int main(){
	hello();
	double x=1;
	double y = sfuns::fgamma(x);
	std::cout << "fgamma(1)=" << y << "\n";
	return 0;
}
