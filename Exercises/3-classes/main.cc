#include<iostream>
#include<cstdio>
#include<vector>
#include<string>
#include<cassert>
#include"hello.h"
#include"vec.h"
#include"epsilon.h"

void print_double(double x, std::string s = ""){
	std::cout << s << x << std::endl;
}

int main(){
	hello();
	epsilon_machine();
	non_commutativity();
	comparing_doubles();


	double da=1.0;
	double db=da;
	if(da==db) {std::cout << "da==db\n";}
	else {std::cout << "da!=db\n";}

	std::vector<double> v {1,2,3};
	std::cout << "for(size_t=0....) loop:\n";
	for(size_t i=0;i<v.size();i++)std::cout << v[i] << " ";

	std::cout << "\nfor(auto vi : v) loop:";
	for(auto vi : v) std::cout << vi << " ";
	std::cout << "\nfor(double vi :v) loop:";
	for(double vi : v) std::cout << vi << " ";
	std::cout << "\n";

	//for(auto vi : v) vi=6;
	//for(auto vi : v)std::cout << vi;
	
	std::cout << "\n";
	for(auto& vi : v) vi=6;
	std::cout << "is v changed after 'auto& vi' ?\n";
	for(auto& vi : v) std::cout << vi << " ";

	std::cout << "\n now comes the while loop:  ";
	size_t i=0;
	while(i<v.size()) {
		std::cout << "v[" << i << "]=" << v[i];
		i+=1;
	}

	std::cout << "\n now comes the do loop:  ";
	i=0;
	do{
		std::cout << "v[" << i << "]=" << v[i];
		i+=1;
	}while(i<v.size());
	std::cout << "\n";
	//auto u=v;
	//auto& w=v;
	
/*	{
		std::cout<<"two constructors are going to be called shortly...\n";
		pp::vec δ(9,9,9);
		pp::vec α{-1,-1,-1};
		δ.x=0;
		std::cout<<"here comes vec δ: "<<δ.x<<" "<<δ.y<<" "<<δ.z<<"\n";
		std::cout<<"calling friend operator<< on vec ";
		std::cout<<"δ: "<<δ<<"\n";
		std::cout<<"here comes vec α: "<<α<<"\n";

		std::cout<<"α+δ: "<< α+δ <<"\n";

		std::cout<<"now a couple of destructors are going to be called...\n";
	}
*/
	// Part 2: Vec tests
    	std::cout << "--- Starting Vec Tests ---\n";
    	vec a(1, 2, 3);
   	vec b(4, 5, 6);

   	// Test modern operator+
   	vec c = a + b; 
   	std::cout << "a + b = " << c << "\n";
    	assert(approx(c.x, 5.0));

    	// Test Math
   	std::cout << "a . b = " << a.dot(b) << " (Expected 32)\n";
    
    	vec e1(1, 0, 0), e2(0, 1, 0);
   	std::cout << "e1 x e2 = " << e1.cross(e2) << " (Expected {0,0,1})\n";

	double n = vec(3, 4, 0).norm();
    	std::cout << "Norm: " << n << " (Expected 5)\n";
    	assert(approx(n, 5.0));
    
	// Test scale
    	std::cout << "a * 2 = " << (a * 2.0) << "\n";

    	std::cout << "\nAll tests passed successfully.\n";

return 0;
}
