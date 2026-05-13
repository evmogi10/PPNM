/* Hold on Gemini 3 */
#include<iostream>
#include<cstdio>
#include<limits>
#include<cmath>
#include<iomanip>
#include "epsilon.h"

void epsilon_machine(){
	 
	float       f=1.0f; while((float)      (1.0f+f) != 1.0f){f/=2.0f;} f*=2.0f;
	double      d=1.0d; while((double)     (1.0d+d) != 1.0d){d/=2.0d;} d*=2.0d;
	long double l=1.0L; while((long double)(1.0L+l) != 1.0L){l/=2.0L;} l*=2.0L;

	std::printf("Manual results\n");
	std::printf("      float eps=%g\n",f);
	std::printf("     double eps=%g\n",d);
	std::printf("long double eps=%Lg\n",l);

	// Comparation with <limits>
	std::cout << "Values of <limits> \n";
	std::cout << std::numeric_limits<float>::epsilon() << "\n";
	std::cout << std::numeric_limits<double>::epsilon() << "\n";
	std::cout << std::numeric_limits<long double>::epsilon() << "\n";

	//Theoretical verification
	std::cout << "Theoretical verification\n";
	std::cout << "pow(2,-23) [float]: " << std::pow(2,-23) << "\n";
	std::cout << "pow(2,-52) [double]: " << std::pow(2,-52) << "\n";

}

void non_commutativity(){

	double epsilon=std::pow(2,-52);
	double tiny=epsilon/2;
	double a=1+tiny+tiny;
	double b=tiny+tiny+1;
	std::cout << "a==b ? " << (a==b ? "true":"false") << "\n";
	std::cout << "a>1  ? " << (a>1  ? "true":"false") << "\n";
	std::cout << "b>1  ? " << (b>1  ? "true":"false") << "\n";

	// Verification with high precision
	std::cout << std::fixed << std::setprecision(17);
	std::cout << "       tiny=" << tiny << "\n";
	std::cout << "1+tiny+tiny=" << a << "\n";
	std::cout << "tiny+tiny+1=" << b << "\n";

// a>1 is false because tiny is smaller than the machine epsilon (pow(2, -53) in this case), the computer cannot "see" the difference when you add it to 1.0. The result of 1+tiny is rounded back down to 1.0. Doing this twice still results in 1.0. 
// b>1 is true because the two tiny values are added to each other first. Since they are the same scale, they result in 2xtiny, which equals exactly epsilon (pow(2, -52)). When you then add 1+epsilon, the computer can represent that change.
}

bool approx(double a, double b, double acc = 1e-9, double eps = 1e-9) {
    double diff = std::abs(a - b);
    if (diff <= acc) return true;  // absolute tolerance
    double max_ab = std::max(std::abs(a), std::abs(b));
    return diff <= eps * max_ab;   // relative tolerance
}

void comparing_doubles(){
	double d1 = 0.1+0.1+0.1+0.1+0.1+0.1+0.1+0.1;
	double d2 = 8*0.1;

	std::cout << std::fixed << std::setprecision(17);
	std::cout << "d1=" << d1 << "\n";
	std::cout << "d2=" << d2 << "\n";

	// Direct comparision
	std::cout << "d1==d2? " << (d1==d2 ? "true":"false") << "\n";

	// Comparision using approx
	std::cout << "approx(d1, d2) ? " << approx(d1, d2) << "\n\n";
}



