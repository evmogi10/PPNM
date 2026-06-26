/* Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested. */
#include "matrix.h"
#include "jacobi.h"
#include "lanczos.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <cmath>

// Generates the finite difference discretization matrix for the Hydrogen atom
pp::matrix make_hydrogen_hamiltonian(int N, double dr) {
    pp::matrix H(N, N);
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) H(i, j) = 0.0;
    }
    for (int i = 0; i < N; i++) {
        double r = (i + 1) * dr;
        H(i, i) = 1.0 / (dr * dr) - 2.0 / r;
        if (i > 0)     H(i, i - 1) = -0.5 / (dr * dr);
        if (i < N - 1) H(i, i + 1) = -0.5 / (dr * dr);
    }
    return H;
}

// Generates a famous Wilkinson W21+ Matrix to explicitly test edge cases
pp::matrix make_wilkinson_21() {
    const int n = 21;
    pp::matrix W(n, n);
    for(int i=0; i<n; i++) for(int j=0; j<n; j++) W(i,j) = 0.0;

    for (int i = 0; i < n; i++) {
        W(i, i) = std::abs(10 - i);
        if (i > 0)     W(i, i - 1) = 1.0;
        if (i < n - 1) W(i, i + 1) = 1.0;
    }
    return W;
}

int main() {
    std::cout << "========================================================\n";
    std::cout << "      EXAMINATION PROJECT: LANCZOS TRIDIAGONALIZATION   \n";
    std::cout << "========================================================\n\n";

    // -------------------------------------------------------------------------
    // THEORETICAL CONTEXT & REAL-WORLD APPLICATIONS (From Wikipedia)
    // -------------------------------------------------------------------------
    std::cout << "[THEORETICAL CONTEXT]\n";
    std::cout << "This implementation utilizes Full Gram-Schmidt Reorthogonalization\n";
    std::cout << "to stabilize the raw Lanczos method (Ojalvo & Newman, 1970).\n\n";
    std::cout << "Advanced Variations in the Field:\n";
    std::cout << "  - Implicitly Restarted Lanczos (used in ARPACK/MATLAB/SciPy)\n";
    std::cout << "  - Block Lanczos (used for parallel multicore fetching)\n\n";
    std::cout << "Real-World Applications:\n";
    std::cout << "  - Quantum Mechanics (Shell models, strongly correlated electrons)\n";
    std::cout << "  - Data Science (Google's PageRank, HITS Algorithm, Latent Semantic Indexing)\n";
    std::cout << "========================================================\n\n";

    // -------------------------------------------------------------------------
    // ALGORITHMIC VALIDATION VIA WILKINSON MATRIX
    // -------------------------------------------------------------------------
    std::cout << "Validating Algorithm via Wilkinson W21+ Matrix:\n";
    pp::matrix W = make_wilkinson_21();
    pp::EVD wilk_jacobi(W);

    double max_eigen_jacobi = wilk_jacobi.w[0];
    for(int i=1; i<21; i++) if(wilk_jacobi.w[i] > max_eigen_jacobi) max_eigen_jacobi = wilk_jacobi.w[i];

    pp::LanczosResult wilk_lanczos = pp::lanczos_tridiag(W, 10);
    pp::EVD wilk_sub_evd(wilk_lanczos.T);
    double max_eigen_lanczos = wilk_sub_evd.w[0];
    for(int i=1; i<10; i++) if(wilk_sub_evd.w[i] > max_eigen_lanczos) max_eigen_lanczos = wilk_sub_evd.w[i];

    std::cout << "  - True Max Eigenvalue (Full Jacobi):      " << max_eigen_jacobi << "\n";
    std::cout << "  - Compressed Max Eigenvalue (Lanczos n=10): " << max_eigen_lanczos << "\n";
    std::cout << "  - Absolute Precision Error:               " << std::abs(max_eigen_jacobi - max_eigen_lanczos) << "\n";
    std::cout << "  - Krylov Basis Ortho Deviation:           " << std::scientific << wilk_lanczos.max_ortho_deviation << "\n\n";

    // -------------------------------------------------------------------------
    // RADIAL HYDROGEN ATOM SIMULATION
    // -------------------------------------------------------------------------
    std::cout << "========================================================\n";
    std::cout << "         HYDROGEN RADIAL DISCRETIZATION STUDY           \n";
    std::cout << "========================================================\n";
    const int N = 200;
    const double dr = 0.1;

    std::cout << "System Parametrization:\n";
    std::cout << "  - Matrix Dimension (N): " << N << "\n";
    std::cout << "  - Radial Grid Step (dr): " << dr << " a.u.\n";
    std::cout << "  - Boundary Radius (rmax): " << N * dr << " a.u.\n\n";

    pp::matrix H = make_hydrogen_hamiltonian(N, dr);

    std::cout << "Running baseline Full Jacobi Diagonalization... ";
    std::cout.flush();
    auto t0 = std::chrono::high_resolution_clock::now();
    pp::EVD full_evd(H);
    auto t1 = std::chrono::high_resolution_clock::now();
    double time_jacobi = std::chrono::duration<double>(t1 - t0).count();

    double ground_jacobi = full_evd.w[0];
    for (int i = 1; i < N; i++) {
        if (full_evd.w[i] < ground_jacobi) ground_jacobi = full_evd.w[i];
    }
    std::cout << "Done.\n";
    std::cout << "  - Full Jacobi Ground State: " << std::setprecision(8) << ground_jacobi << " Hartree\n";
    std::cout << "  - Total Compute Time: " << time_jacobi << " seconds\n\n";

    std::cout << "Running Lanczos iterative Subspace Compression...\n";
    std::ofstream conv_file("convergence.txt");
    std::ofstream time_file("timing.txt");

    conv_file << "# n   Lanczos_E0       Absolute_Error\n";
    time_file << "# n   Lanczos_Time_s   Complexity_Ratio_O(d*N*n + N*n^2)\n";

    // -------------------------------------------------------------------------
    // EXPERIMENTAL VALIDATION OF O(d*n*m) COMPLEXITY
    // -------------------------------------------------------------------------

    std::cout << " Validating True Time Complexity Scaling:\n";
    std::cout << "  (Raw Lanczos is O(d*N*n), but Full Reorthogonalization makes it O(d*N*n + N*n^2))\n";

    for (int n_sub = 5; n_sub <= 60; n_sub += 5) {
        auto start_l = std::chrono::high_resolution_clock::now();

        pp::LanczosResult res = pp::lanczos_tridiag(H, n_sub);
        pp::EVD sub_evd(res.T);

        auto end_l = std::chrono::high_resolution_clock::now();
        double time_l = std::chrono::duration<double>(end_l - start_l).count();

        double ground_lanczos = sub_evd.w[0];
        for (int i = 1; i < n_sub; i++) {
            if (sub_evd.w[i] < ground_lanczos) ground_lanczos = sub_evd.w[i];
        }

        double error = std::abs(ground_lanczos - ground_jacobi);

	// Calculate True Complexity Ratio
        // Raw Lanczos is O(d * N * n), but Full Gram-Schmidt Reorthogonalization adds O(N * n^2).
        // If our theory holds, Time / (d*N*n + N*n^2) should be relatively constant.
        int d = 3;
        double theoretical_operations = (d * N * n_sub) + (N * n_sub * n_sub);
        double complexity_ratio = time_l / theoretical_operations;

        conv_file << n_sub << " " << std::scientific << ground_lanczos << " " << error << "\n";
        time_file << n_sub << " " << std::scientific << time_l << " " << complexity_ratio << "\n";

        std::cout << "  - n = " << std::setw(2) << n_sub
                  << " | E0 = " << std::fixed << std::setprecision(6) << ground_lanczos
                  << " | Error: " << std::scientific << error
                  << " | O(d*N*n + N*n^2) Ratio: " << complexity_ratio << "\n";
    }
    conv_file.close();
    time_file.close();

    std::cout << "\nData pipelines successfully mapped to 'convergence.txt' and 'timing.txt'.\n";
    return 0;
}
