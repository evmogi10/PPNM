#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include "matrix.h"
#include "jacobi.h"

int main(int argc, char** argv) {
	
    // --- PARTE 3: BENCHMARK O(N^3) ---
    if (argc == 2 && std::isdigit(argv[1][0])) {
        int N_bench = std::stoi(argv[1]);
        pp::matrix A_bench(N_bench, N_bench);
        for(int r=0; r<N_bench; r++) {
            for(int c=r; c<N_bench; c++) {
                A_bench(r,c) = A_bench(c,r) = (double)std::rand()/RAND_MAX;
            }
        }
        pp::EVD evd(A_bench); 
        return 0; // Terminar aquí para que 'time' mida solo esto
    }

    double rmax = 10.0;
    double dr = 0.3;
    bool do_hydrogen = false;
    bool print_wave = false;
    bool print_energy = false;

    // Procesar argumentos de la terminal (para el Makefile)
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-rmax" && i+1 < argc) { rmax = std::stod(argv[++i]); do_hydrogen = true; }
        if (arg == "-dr" && i+1 < argc) { dr = std::stod(argv[++i]); do_hydrogen = true; }
        if (arg == "-wave") { print_wave = true; do_hydrogen = true; }
        if (arg == "-energy") { print_energy = true; do_hydrogen = true; }
    }

    if (do_hydrogen) {
        // --- PARTE 2: ÁTOMO DE HIDRÓGENO ---
        int npoints = (int)(rmax / dr) - 1;
        pp::vector r(npoints);
        for (int i = 0; i < npoints; i++) r[i] = dr * (i + 1); // r[i] en tu vector

        pp::matrix H(npoints, npoints);
        for(int i=0; i<npoints; i++) for(int j=0; j<npoints; j++) H(i,j) = 0.0;

        // Llenar matriz tridiagonal (Energía Cinética K)
        for (int i = 0; i < npoints - 1; i++) {
            H(i, i) = -2 * (-0.5 / (dr * dr));
            H(i, i + 1) = 1 * (-0.5 / (dr * dr));
            H(i + 1, i) = 1 * (-0.5 / (dr * dr));
        }
        H(npoints - 1, npoints - 1) = -2 * (-0.5 / (dr * dr));

        // Añadir Energía Potencial W en la diagonal
        for (int i = 0; i < npoints; i++) {
            H(i, i) += -1.0 / r[i];
        }

        // ¡Magia! Diagonalizamos
        pp::EVD evd(H);

        if (print_energy) {
            // Imprime E0 (el primer autovalor)
            std::cout << dr << " " << rmax << " " << evd.w[0] << "\n";
        }

        if (print_wave) {
            double norm = 1.0 / std::sqrt(dr);
            for (int i = 0; i < npoints; i++) {
                // Columnas de V son las funciones de onda
                std::cout << r[i] << " " 
                          << evd.V(i, 0) * norm << " " 
                          << evd.V(i, 1) * norm << " " 
                          << evd.V(i, 2) * norm << "\n";
            }
        }
        return 0;
    }

    // --- PARTE 1: PRUEBA DE JACOBI (Sin argumentos) ---
    int N = 5;
    pp::matrix A(N, N);
    for(int i=0; i<N; i++) {
        for(int j=i; j<N; j++) {
            A(i,j) = A(j,i) = (double)rand()/RAND_MAX; // Simétrica aleatoria
        }
    }

    pp::matrix A_orig = A; // Copia
    pp::EVD evd(A); // Factoriza

    A_orig.print("Matriz Original A");
    evd.V.print("Autovectores V");
    evd.w.print("Autovalores w");
    std::cout << "\n";

    // --- COMPROBACIONES (usando tus operadores sobrecargados) ---
    pp::matrix VtAV = evd.V.T() * A_orig * evd.V;
    VtAV.print("Comprobación V^T * A * V (Debe ser diagonal = w)");

    pp::matrix VDVt = evd.V * VtAV * evd.V.T();
    VDVt.print("Comprobación V * D * V^T (Debe ser igual a A_orig)");

    pp::matrix VtV = evd.V.T() * evd.V;
    VtV.print("Comprobación V^T * V (Debe ser la Identidad)");
    
    return 0;
}
