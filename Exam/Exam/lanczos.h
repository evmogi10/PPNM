/* Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested. */
#pragma once
#include "matrix.h"
#include <cmath>
#include <vector>
#include <random>
#include <cassert>
#include <iostream>

namespace pp {

// Pair structure to return both the tridiagonal matrix and its basis orthogonality metric
struct LanczosResult {
    matrix T;
    double max_ortho_deviation;
};

inline LanczosResult lanczos_tridiag(const matrix& A, int n_sub, const vector& v_init = vector()) {
    int N = A.size1();
    assert(A.size1() == A.size2());
    if (n_sub > N) n_sub = N;

    matrix T(n_sub, n_sub);
    for(int i=0; i<n_sub; i++) for(int j=0; j<n_sub; j++) T(i,j) = 0.0;

    matrix V(N, n_sub);

    // 1. Initialize basis vector
    vector v1(N);
    if (v_init.size() == N) {
        v1 = v_init;
    } else {
        std::mt19937 gen(1337); // Fixed seed for reproducible benchmarks
        std::normal_distribution<double> dist(0.0, 1.0);
        for (int i = 0; i < N; i++) v1[i] = dist(gen);
    }
    v1 = v1 / v1.norm();
    for (int i = 0; i < N; i++) V(i, 0) = v1[i];

    // 2. First seed step
    vector w_prime = A * v1;
    double alpha = 0.0;
    for (int i = 0; i < N; i++) alpha += v1[i] * w_prime[i];
    T(0, 0) = alpha;

    vector w = w_prime - v1 * alpha;

    // 3. Iteration Loop
    for (int j = 1; j < n_sub; j++) {
        double beta = w.norm();
        T(j, j - 1) = beta;
        T(j - 1, j) = beta;

        if (beta < 1e-14) {
            std::mt19937 gen(1337 + j);
            std::normal_distribution<double> dist(0.0, 1.0);
            for (int i = 0; i < N; i++) w[i] = dist(gen);
            for (int k = 0; k < j; k++) {
                double dot = 0.0;
                for (int i = 0; i < N; i++) dot += V(i, k) * w[i];
                for (int i = 0; i < N; i++) w[i] -= V(i, k) * dot;
            }
            beta = w.norm();
        }

        for (int i = 0; i < N; i++) V(i, j) = w[i] / beta;

        vector vj(N), vj_minus(N);
        for (int i = 0; i < N; i++) {
            vj[i] = V(i, j);
            vj_minus[i] = V(i, j - 1);
        }

        w_prime = A * vj - vj_minus * beta;

        alpha = 0.0;
        for (int i = 0; i < N; i++) alpha += vj[i] * w_prime[i];
        T(j, j) = alpha;

        w = w_prime - vj * alpha;

        // FULL REORTHOGONALIZATION LOOP
        for (int k = 0; k <= j; k++) {
            double dot = 0.0;
            for (int i = 0; i < N; i++) dot += V(i, k) * w[i];
            for (int i = 0; i < N; i++) w[i] -= V(i, k) * dot;
        }
    }

    // Compute max numerical deviation from identity matrix: ||V^T * V - I||
    double max_dev = 0.0;
    for (int i = 0; i < n_sub; i++) {
        for (int j = 0; j < n_sub; j++) {
            double dot = 0.0;
            for (int k = 0; k < N; k++) dot += V(k, i) * V(k, j);
            double target = (i == j) ? 1.0 : 0.0;
            double dev = std::abs(dot - target);
            if (dev > max_dev) max_dev = dev;
        }
    }

    return LanczosResult{T, max_dev};
}

} // namespace pp
