/* Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .*/
#pragma once
#include<iostream>
#include<cstdio>
#include<cassert>
#include<cstdio>
#include<iomanip>
#include<cmath>
#include<string>
#include<vector>
#include<initializer_list>
#include<functional>
#include<ranges>

namespace pp {

struct qr {
    matrix Q;
    matrix R;

    // Constructor: Performs QR decomposition of A
    qr(const matrix& A) : Q(A.size1(), A.size2()), R(A.size2(), A.size2()) {
        int n = A.size1();
        int m = A.size2();
        assert(n >= m);

        // Copy A into Q to begin the stabilized process
        Q = A;

        for (int i = 0; i < m; i++) {
            // R(i,i) is the norm of the i-th column of the current Q
            double norm = Q[i].norm();
            if(norm > 1e-15){
	    	R(i, i) = norm;

            	// Normalize the i-th column of Q
            	Q[i] /= norm;
	    } else{
	    	R(i,i) = 0;
	    }

            for (int j = i + 1; j < m; j++) {
                // R(i,j) is the projection of the j-th column onto the i-th column
                // Since Q[i] is now orthonormal, R(i,j) = Q[i] dot Q[j]
                double s = 0;
                for (int k = 0; k < n; k++) s += Q(k, i) * Q(k, j);
                R(i, j) = s;

                // Stabilized step: subtract the projection from the j-th column
                // Q[j] = Q[j] - Q[i] * R(i,j)
                for (int k = 0; k < n; k++) Q(k, j) -= Q(k, i) * s;
            }
        }
    }

    vector solve(vector b) const {
    // 1. Calculate y = Qt * b
    vector y = Q.transpose() * b;

    // 2. Back-substitution to solve Rx = y
    int m = R.size2();
    vector x(m);

    for (int i = m - 1; i >= 0; i--) {
        double sum = 0;
        for (int j = i + 1; j < m; j++) {
            sum += R(i, j) * x[j];
        }
        x[i] = (y[i] - sum) / R(i, i);
    }
    return x;
    }


    double det() const;

    matrix inverse() const;

};

} // namespace pp
