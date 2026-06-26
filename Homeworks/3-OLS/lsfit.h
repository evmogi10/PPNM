/* Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .*/
#pragma once
#include "matrix.h"
#include "qr.h"
#include <vector>
#include <functional>
#include <tuple>

namespace pp {

inline std::tuple<pp::vector, pp::matrix> lsfit(
    const std::vector<std::function<double(double)>>& fs,
    const pp::vector& x,
    const pp::vector& y,
    const pp::vector& dy)
{
    int n = x.size();
    int m = fs.size();

    pp::matrix A(n, m);
    pp::vector b(n);

    // Build the weighted overdetermined system A*c = b
    for (int i = 0; i < n; i++) {
        b[i] = y[i] / dy[i];
        for (int k = 0; k < m; k++) {
            A(i, k) = fs[k](x[i]) / dy[i];
        }
    }

    // Solve using your QR decomposition
    pp::qr solver(A);
    pp::vector c = solver.solve(b);

    // Calculate Covariance Matrix: Sigma = (A^T * A)^-1 = (R^T * R)^-1 = R^-1 * (R^-1)^T
    // Since R is an m x m upper triangular matrix, we can invert it easily:
    pp::matrix R_inv(m, m);
    for(int i=0; i<m; i++) for(int j=0; j<m; j++) R_inv(i,j) = 0.0;

    for (int j = 0; j < m; j++) {
        R_inv(j, j) = 1.0 / solver.R(j, j);
        for (int i = j - 1; i >= 0; i--) {
            double sum = 0;
            for (int k = i + 1; k <= j; k++) {
                sum += solver.R(i, k) * R_inv(k, j);
            }
            R_inv(i, j) = -sum / solver.R(i, i);
        }
    }

    pp::matrix Sigma = R_inv * R_inv.transpose();

    return std::make_tuple(c, Sigma);
}

} // namespace pp
