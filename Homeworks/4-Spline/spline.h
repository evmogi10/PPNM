/* Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .*/
#pragma once
#include "matrix.h"
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <utility>

namespace pp {

// --- BINARY SEARCH ---
inline int binsearch(const pp::vector& x, double z) {
    if (z < x[0] || z > x[x.size() - 1]) {
        throw std::invalid_argument("Interpolation point z out of bounds.");
    }
    int i = 0, j = x.size() - 1;
    while (j - i > 1) {
        int mid = (i + j) / 2;
        if (z >= x[mid]) i = mid; else j = mid;
    }
    return i;
}

// --- TASK A: LINEAR SPLINE ---
inline double linterp(const pp::vector& x, const pp::vector& y, double z) {
    int i = binsearch(x, z);
    double dx = x[i+1] - x[i];
    double dy = y[i+1] - y[i];
    return y[i] + dy / dx * (z - x[i]);
}

inline double linterpInteg(const pp::vector& x, const pp::vector& y, double z) {
    int idx = binsearch(x, z);
    double sum = 0;
    // Full intervals
    for (int i = 0; i < idx; i++) {
        double dx = x[i+1] - x[i];
        double dy = y[i+1] - y[i];
        sum += y[i] * dx + 0.5 * dy * dx;
    }
    // Partial interval up to z
    double dx_z = z - x[idx];
    double dy_z = (y[idx+1] - y[idx]) / (x[idx+1] - x[idx]) * dx_z;
    sum += y[idx] * dx_z + 0.5 * dy_z * dx_z;
    return sum;
}

// --- TASK B: QUADRATIC SPLINE ---
struct qspline {
    int n;
    pp::vector x, y, b, c;

    qspline(const pp::vector& x, const pp::vector& y) 
        : n(x.size()), x(x), y(y), b(n-1), c(n-1) 
    {
        pp::vector dx(n-1), p(n-1);
        for (int i = 0; i < n - 1; i++) {
            dx[i] = x[i+1] - x[i];
            p[i] = (y[i+1] - y[i]) / dx[i];
        }

        // Forward recursion
        pp::vector c_f(n-1);
        c_f[0] = 0.0;
        for (int i = 0; i < n - 2; i++) {
            c_f[i+1] = (p[i+1] - p[i] - c_f[i] * dx[i]) / dx[i+1];
        }

        // Backward recursion (To prevent asymmetric error accumulation)
        pp::vector c_b(n-1);
        c_b[n-2] = 0.0;
        for (int i = n - 3; i >= 0; i--) {
            c_b[i] = (p[i+1] - p[i] - c_b[i+1] * dx[i+1]) / dx[i];
        }

        // Average forward and backward to get c, then compute b
        for (int i = 0; i < n - 1; i++) {
            c[i] = (c_f[i] + c_b[i]) / 2.0;
            b[i] = p[i] - c[i] * dx[i];
        }
    }

    double eval(double z) const {
        int i = binsearch(x, z);
        double dz = z - x[i];
        return y[i] + b[i] * dz + c[i] * dz * dz;
    }

    double deriv(double z) const {
        int i = binsearch(x, z);
        double dz = z - x[i];
        return b[i] + 2.0 * c[i] * dz;
    }

    double integ(double z) const {
        int idx = binsearch(x, z);
        double sum = 0;
        for (int i = 0; i < idx; i++) {
            double dx = x[i+1] - x[i];
            sum += y[i] * dx + b[i] * dx * dx / 2.0 + c[i] * dx * dx * dx / 3.0;
        }
        double dz = z - x[idx];
        sum += y[idx] * dz + b[idx] * dz * dz / 2.0 + c[idx] * dz * dz * dz / 3.0;
        return sum;
    }
};


// --- TASK C (Part 1): FUNCTIONAL QUADRATIC SPLINE ---
inline auto make_qspline_eval(pp::vector x_in, pp::vector y_in) {
    int n = x_in.size();
    pp::vector b(n - 1);
    pp::vector c(n - 1);

    // 1. Calculate dx and p
    pp::vector dx(n - 1), p(n - 1);
    for (int i = 0; i < n - 1; i++) {
        dx[i] = x_in[i+1] - x_in[i];
        p[i] = (y_in[i+1] - y_in[i]) / dx[i];
    }

    // 2. Forward recursion for c
    pp::vector c_f(n - 1);
    c_f[0] = 0.0;
    for (int i = 0; i < n - 2; i++) {
        c_f[i+1] = (p[i+1] - p[i] - c_f[i] * dx[i]) / dx[i+1];
    }

    // 3. Backward recursion for c
    pp::vector c_b(n - 1);
    c_b[n-2] = 0.0;
    for (int i = n - 3; i >= 0; i--) {
        c_b[i] = (p[i+1] - p[i] - c_b[i+1] * dx[i+1]) / dx[i];
    }

    // 4. Average and calculate b
    for (int i = 0; i < n - 1; i++) {
        c[i] = (c_f[i] + c_b[i]) / 2.0;
        b[i] = p[i] - c[i] * dx[i];
    }

    // 5. Return the lambda capturing by move
    return [x = std::move(x_in), y = std::move(y_in), b = std::move(b), c = std::move(c)](double z) {
        int i = binsearch(x, z);
        double dz = z - x[i];
        return y[i] + b[i] * dz + c[i] * dz * dz;
    };

}

// --- TASK C (Part 2): CUBIC SPLINE ---
struct cspline {
    int n;
    pp::vector x, y, b, c, d;

    cspline(const pp::vector& x, const pp::vector& y) 
        : n(x.size()), x(x), y(y), b(n), c(n-1), d(n-1) 
    {
        pp::vector h(n-1), p(n-1);
        for (int i = 0; i < n - 1; i++) {
            h[i] = x[i+1] - x[i];
            p[i] = (y[i+1] - y[i]) / h[i];
        }

        // Build tridiagonal system for natural cubic spline
        pp::vector D(n), Q(n-1), B(n);
        D[0] = 2.0; Q[0] = 1.0; B[0] = 3.0 * p[0];
        for (int i = 1; i < n - 1; i++) {
            D[i] = 2.0 * h[i-1] / h[i] + 2.0;
            Q[i] = 1.0;
            B[i] = 3.0 * (p[i-1] + p[i] * h[i-1] / h[i]);
        }
        D[n-1] = 2.0; B[n-1] = 3.0 * p[n-2];

        // Thomas algorithm (Forward elimination)
        for (int i = 1; i < n; i++) {
            D[i] -= Q[i-1] / D[i-1];
            B[i] -= B[i-1] / D[i-1];
        }
        
        // Back-substitution for b
        b[n-1] = B[n-1] / D[n-1];
        for (int i = n - 2; i >= 0; i--) {
            b[i] = (B[i] - Q[i] * b[i+1]) / D[i];
        }

        // Calculate c and d
        for (int i = 0; i < n - 1; i++) {
            c[i] = (-2.0 * b[i] - b[i+1] + 3.0 * p[i]) / h[i];
            d[i] = (b[i] + b[i+1] - 2.0 * p[i]) / (h[i] * h[i]);
        }
    }

    double eval(double z) const {
        int i = binsearch(x, z);
        double dz = z - x[i];
        return y[i] + b[i] * dz + c[i] * dz * dz + d[i] * dz * dz * dz;
    }

    double deriv(double z) const {
        int i = binsearch(x, z);
        double dz = z - x[i];
        return b[i] + 2.0 * c[i] * dz + 3.0 * d[i] * dz * dz;
    }

    double integ(double z) const {
        int idx = binsearch(x, z);
        double sum = 0;
        for (int i = 0; i < idx; i++) {
            double dx = x[i+1] - x[i];
            sum += y[i]*dx + b[i]*dx*dx/2.0 + c[i]*dx*dx*dx/3.0 + d[i]*dx*dx*dx*dx/4.0;
        }
        double dz = z - x[idx];
        sum += y[idx]*dz + b[idx]*dz*dz/2.0 + c[idx]*dz*dz*dz/3.0 + d[idx]*dz*dz*dz*dz/4.0;
        return sum;
    }
};

} // namespace pp
