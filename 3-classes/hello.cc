#include"hello.h"
#include<iostream>
void hello(void){
	std::cout << "Hello, World!" << "\n";
	double x=666;
	{
		double x=1; // shadowing
		std::cout << x << "\n";
	}
	std::cout << x << "\n";
}
