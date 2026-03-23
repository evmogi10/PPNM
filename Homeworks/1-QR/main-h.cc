#include "matrix.h"
#include "qr.h"
#include <iostream>
#include <cstdlib>
#include <string>

int main(int argc, char** argv) {

    // 1. Check if we are in "Timing Mode"
    if (argc > 2 && std::string(argv[1]) == "-size") {
        int N = std::stoi(argv[2]);
        
        // Generate random square matrix
        pp::matrix A(N, N);
        for(int i = 0; i < N; i++)
            for(int j = 0; j < N; j++)
                A(i, j) = (double)rand() / RAND_MAX;

        // ONLY measure the factorization
        pp::qr solver(A); 
        
    	return 0; // Exit silently so 'time' utility can record the duration
    }	

    // --- PART 1: QR DECOMPOSITION TEST (TALL MATRIX) ---
    int n1 = 5, m1 = 3;
    pp::matrix A1(n1, m1);

    for(int i=0; i<n1; i++)
        for(int j=0; j<m1; j++)
            A1(i,j) = (double)rand() / RAND_MAX;

    A1.print("Original Matrix A1:");

    pp::qr solver(A1);
    solver.Q.print("Matrix Q:");
    solver.R.print("Matrix R:");

    // Check if R is upper triangular
    bool upper = true;
    for(int i=0; i<m1; i++)
        for(int j=0; j<i; j++)
            if(!pp::approx(solver.R(i,j), 0.0)) upper = false;
    
    std::cout << "Is R upper triangular? " << (upper ? "YES" : "NO") << "\n";

    // Check if Q^T * Q = I
    pp::matrix QtQ = solver.Q.transpose() * solver.Q;
    pp::matrix I_m(m1, m1); 
    I_m.setid();
    std::cout << "Does Q^T * Q = I? " << (pp::approx(QtQ, I_m) ? "YES" : "NO") << "\n";

    // Check if Q * R = A
    pp::matrix QR = solver.Q * solver.R;
    std::cout << "Does Q * R = A? " << (pp::approx(QR, A1) ? "YES" : "NO") << "\n";


    // --- PART 2: SOLVER TEST (SQUARE MATRIX) ---
    // Note: I renamed 'n' to 'n2' and 'A' to 'A2' to avoid redefinition errors
    int n2 = 5; 
    pp::matrix A2(n2, n2);
    pp::vector b(n2);

    for (int i = 0; i < n2; i++) {
        b[i] = (double)(rand() % 10 + 1);
        for (int j = 0; j < n2; j++) {
            A2(i, j) = (double)(rand() % 10 + 1);
        }
    }

    // Factorize square matrix
    pp::qr sol2(A2);

    // Solve QRx = b
    pp::vector x = sol2.solve(b);

    // Check Ax = b
    pp::vector Ax = A2 * x;
    bool check3 = pp::approx(Ax, b, 1e-8);

    std::cout << "\nCHECK 3: Does Ax = b? " << (check3 ? "YES" : "NO") << "\n";
    if (!check3) {
        Ax.print("Ax (Result): ");
        b.print("b (Target): ");
    }
    
    // --- PART 3: DETERMINANT ---
    pp::matrix A_det(2, 2);
    A_det(0,0) = 4; A_det(0,1) = 3;
    A_det(1,0) = 3; A_det(1,1) = 2;
    // Expected det = (4*2) - (3*3) = -1

    pp::qr sol_det(A_det);
    double my_det = sol_det.det();

    std::cout << "--- Determinant Test ---" << std::endl;
    std::cout << "Calculated det(A): " << my_det << std::endl;
    std::cout << "Expected det(A):   -1 (or 1 depending on Q reflection)" << std::endl;



    // --- PART 4: INVERSE TEST ---
    std::cout << "\n--- Testing Matrix Inverse ---" << std::endl;
    int n_inv = 4;
    pp::matrix A_inv(n_inv, n_inv);

    // Fill with random data
    for(int i=0; i<n_inv; i++)
	    for(int j=0; j<n_inv; j++)
		    A_inv(i,j) = (double)(rand() % 10 + 1);

    pp::qr sol_inv(A_inv);
    pp::matrix B = sol_inv.inverse();

    // Check if A * B = I
    pp::matrix AB = A_inv * B;
    pp::matrix Identity(n_inv, n_inv);
    Identity.setid();

    bool inv_check = pp::approx(AB, Identity, 1e-7);
    std::cout << "Does A * A^-1 = I? " << (inv_check ? "YES" : "NO") << "\n";

    if(!inv_check) {
	    AB.print("A * B result (should be Identity):");
    }


    return 0;
}
