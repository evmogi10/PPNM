#include<iostream>
#include<fstream>
#include<string>
#include<vector>
int main (int argc, char *argv[]) {
	std::string infile="", outfile="";
	for(int i=0;i<argc;i++){
		std::string arg=argv[i];
		if(arg=="--input" && i+1 < argc) infile=argv[i+1];
		if(arg=="--output" && i+1 < argc) outfile=argv[i+1];
	}
std::ifstream myinput(infile);
std::ofstream myoutput(outfile);
double x;
if( myinput.is_open() && myoutput.is_open() ){
	while( myinput >> x ){
		myoutput << x <<" "<<std::sin(x)<<" "<<std::cos(x)<<std::endl;
		}
	}
else{
	std::cerr << "Error opening files: " << infile << outfile << std::endl;
	return EXIT_FAILURE;
    }
myinput.close();
myoutput.close();
exit(EXIT_SUCCESS);
}
/*int main(int argc, char** argv){
	std::string arg,infile;
	double dr=0.1,rmax=10,number;
	std::vector<double> xvec;
	for(int i=0;i<argc;i++){
		arg=argv[i];
		//std::cout << arg << "\n";
		if(arg=="-dr" && i+1<argc) dr=std::stod(argv[i+1]);
		if(arg=="-rmax" && i+1<argc) rmax=std::stod(argv[i+1]);
		if(arg=="-input" && i+1<argc) infile=argv[i+1];
		if(arg=="-n" && i+1<argc){
			number=std::stod(argv[i+1]);
			xvec.push_back(number);
		}

	}

	std::cout<<"dr="<<dr<<" rmax="<<rmax<<"\n";
	for(const double& xi : xvec) std::cout<<xi<<"\n";
	std::ifstream myinput(infile);
	while(myinput >> number) std::cout << "got this number:" <<number<<"\n";
return 0;
*/
}

