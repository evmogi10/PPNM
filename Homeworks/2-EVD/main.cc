/* Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .*/
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include "matrix.h"
#include "jacobi.h"

int main(int argc, char** argv) {
	
    // --- PART 3: BENCHMARK O(N^3) ---
    if (argc == 2 && std::isdigit(argv[1][0])) {
        int N_bench = std::stoi(argv[1]);
        pp::matrix A_bench(N_bench, N_bench);
        for(int r=0; r<N_bench; r++) {
            for(int c=r; c<N_bench; c++) {
                A_bench(r,c) = A_bench(c,r) = (double)std::rand()/RAND_MAX;
            }
        }
        pp::EVD evd(A_bench); 
        return 0;
    }

    double rmax = 10.0;
    double dr = 0.3;
    bool do_hydrogen = false;
    bool print_wave = false;
    bool print_energy = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-rmax" && i+1 < argc) { rmax = std::stod(argv[++i]); do_hydrogen = true; }
        if (arg == "-dr" && i+1 < argc) { dr = std::stod(argv[++i]); do_hydrogen = true; }
        if (arg == "-wave") { print_wave = true; do_hydrogen = true; }
        if (arg == "-energy") { print_energy = true; do_hydrogen = true; }
    }

    if (do_hydrogen) {
        // --- PART 2: HYDROGEN ATOM ---
        int npoints = (int)(rmax / dr) - 1;
        pp::vector r(npoints);
        for (int i = 0; i < npoints; i++) r[i] = dr * (i + 1); // r[i]

        pp::matrix H(npoints, npoints);
        for(int i=0; i<npoints; i++) for(int j=0; j<npoints; j++) H(i,j) = 0.0;

        for (int i = 0; i < npoints - 1; i++) {
            H(i, i) = -2 * (-0.5 / (dr * dr));
            H(i, i + 1) = 1 * (-0.5 / (dr * dr));
            H(i + 1, i) = 1 * (-0.5 / (dr * dr));
        }
        H(npoints - 1, npoints - 1) = -2 * (-0.5 / (dr * dr));

        for (int i = 0; i < npoints; i++) {
            H(i, i) += -1.0 / r[i];
        }

        pp::EVD evd(H);

        if (print_energy) {
            std::cout << dr << " " << rmax << " " << evd.w[0] << "\n";
        }

        if (print_wave) {
            double norm = 1.0 / std::sqrt(dr);
            for (int i = 0; i < npoints; i++) {
                std::cout << r[i] << " " 
                          << evd.V(i, 0) * norm << " " 
                          << evd.V(i, 1) * norm << " " 
                          << evd.V(i, 2) * norm << "\n";
            }
        }
        return 0;
    }

    // --- PART 1: TEST OF JACOBI (Without args) ---
    int N = 5;
    pp::matrix A(N, N);
    for(int i=0; i<N; i++) {
        for(int j=i; j<N; j++) {
            A(i,j) = A(j,i) = (double)rand()/RAND_MAX;
        }
    }

    pp::matrix A_orig = A;
    pp::EVD evd(A);

    A_orig.print("Original Matrix A");
    evd.V.print("Autovectors V");
    evd.w.print("Autovalues w");
    std::cout << "\n";

    pp::matrix VtAV = evd.V.T() * A_orig * evd.V;
    VtAV.print("Check V^T * A * V (Must be diagonal = w)");

    pp::matrix VDVt = evd.V * VtAV * evd.V.T();
    VDVt.print("Check V * D * V^T (Must be = A_orig)");

    pp::matrix VtV = evd.V.T() * evd.V;
    VtV.print("Check V^T * V (Must be = Identity)");
    
    return 0;
}
