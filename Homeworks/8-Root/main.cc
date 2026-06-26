/* Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .*/
#include <iostream>
#include <fstream>
#include <cmath>
#include "roots.h"
#include "ode.h"

int main() {
    std::cout << "--- Part 1: Rosenbrock's Valley Function ---\n";
    // Roots of the gradient of f(x,y) = (1-x)^2 + 100(y-x^2)^2
    auto grad_rosenbrock = [](pp::vector v) -> pp::vector {
        double x = v[0], y = v[1];
        pp::vector g(2);
        g[0] = -2.0 * (1.0 - x) - 400.0 * x * (y - x * x);
        g[1] = 200.0 * (y - x * x);
        return g;
    };
    
    pp::vector start1 = {-1.0, 2.0};
    pp::vector root_rb = pp::newton(grad_rosenbrock, start1, 1e-5);
    std::cout << "Starting at (-1, 2). Found minimum at: ";
    root_rb.print();
    std::cout << "(Exact minimum is at 1, 1)\n\n";

    std::cout << "--- Part 2: Himmelblau's Function ---\n";
    // Roots of the gradient of f(x,y) = (x^2+y-11)^2 + (x+y^2-7)^2
    auto grad_himmelblau = [](pp::vector v) -> pp::vector {
        double x = v[0], y = v[1];
        pp::vector g(2);
        g[0] = 4.0 * x * (x * x + y - 11.0) + 2.0 * (x + y * y - 7.0);
        g[1] = 2.0 * (x * x + y - 11.0) + 4.0 * y * (x + y * y - 7.0);
        return g;
    };

    pp::vector starts[] = {{3, 2}, {-2.5, 3}, {-3, -3}, {2.5, -2}};
    for (int i = 0; i < 4; i++) {
        pp::vector root_hb = pp::newton(grad_himmelblau, starts[i], 1e-5);
        std::cout << "Found minimum at: ";
        root_hb.print();
    }
    std::cout << "(Exact minima near: (3,2), (-2.8,3.13), (-3.77,-3.28), (3.58,-1.84))\n\n";

    std::cout << "--- Part 3: Bound States of Hydrogen (Shooting Method) ---\n";
    
    // M(E) returns the value of the wavefunction at r_max given energy E.
    auto M = [](double E, double rmin, double rmax, double acc, double eps) -> double {
        auto schrodinger = [E](double r, pp::vector y) -> pp::vector {
            pp::vector dy(2);
            dy[0] = y[1];
            dy[1] = -2.0 * (E + 1.0 / r) * y[0];
            return dy;
        };
        pp::vector yinit(2);
        yinit[0] = rmin - rmin * rmin;
        yinit[1] = 1.0 - 2.0 * rmin;
        
        auto [rlist, ylist] = pp::driver(schrodinger, rmin, rmax, yinit, 0.01, acc, eps);
        return ylist.back()[0]; 
    };

    double rmin = 1e-3, rmax = 8.0, ode_acc = 1e-4, ode_eps = 1e-4;
    
    // Wrapper to make it compatible with our vector-based Newton solver
    auto root_eq = [&](pp::vector vE) -> pp::vector {
        pp::vector res(1);
        res[0] = M(vE[0], rmin, rmax, ode_acc, ode_eps);
        return res;
    };

    // Find the ground state energy E0
    pp::vector E_start = {-1.0}; // Guessing a negative energy
    pp::vector E0_vec = pp::newton(root_eq, E_start, 1e-4);
    double E0 = E0_vec[0];
    
    std::cout << "Ground state energy E0 = " << E0 << " (Exact: -0.5)\n";

    // Re-run the ODE with the found energy to extract the wavefunction for plotting
    auto schrodinger_final = [E0](double r, pp::vector y) -> pp::vector {
        pp::vector dy(2);
        dy[0] = y[1];
        dy[1] = -2.0 * (E0 + 1.0 / r) * y[0];
        return dy;
    };
    pp::vector yinit_final = {rmin - rmin * rmin, 1.0 - 2.0 * rmin};
    auto [r_plot, y_plot] = pp::driver(schrodinger_final, rmin, rmax, yinit_final, 0.01, ode_acc, ode_eps);

    std::ofstream file("hydrogen_wave.txt");
    for (size_t i = 0; i < r_plot.size(); ++i) {
        double r = r_plot[i];
        double f_exact = r * std::exp(-r);
        file << r << " " << y_plot[i][0] << " " << f_exact << "\n";
    }
    file.close();
    std::cout << "Wavefunction data saved to hydrogen_wave.txt\n\n";

    std::cout << "--- Investigating Convergence of E0 ---\n";
    std::cout << "Varying rmax:\n";
    for(double test_rmax = 4.0; test_rmax <= 10.0; test_rmax += 2.0) {
        auto eq = [&](pp::vector vE) -> pp::vector { return {M(vE[0], 1e-3, test_rmax, 1e-4, 1e-4)}; };
        std::cout << "  rmax = " << test_rmax << " -> E0 = " << pp::newton(eq, {-1.0})[0] << "\n";
    }
    
    std::cout << "Varying rmin:\n";
    for(double test_rmin : {0.1, 0.01, 0.001, 0.0001}) {
        auto eq = [&](pp::vector vE) -> pp::vector { return {M(vE[0], test_rmin, 8.0, 1e-4, 1e-4)}; };
        std::cout << "  rmin = " << test_rmin << " -> E0 = " << pp::newton(eq, {-1.0})[0] << "\n";
    }

    std::cout << "Varying acc (with eps = 1e-4):\n";
    for(double test_acc : {1e-1, 1e-2, 1e-3, 1e-4, 1e-5}) {
        // Keeping rmin=1e-3, rmax=8.0, eps=1e-4 constant
        auto eq = [&](pp::vector vE) -> pp::vector { return {M(vE[0], 1e-3, 8.0, test_acc, 1e-4)}; };
        std::cout << "  acc = " << test_acc << " -> E0 = " << pp::newton(eq, {-1.0}, 1e-4)[0] << "\n";
    }

    std::cout << "Varying eps (with acc = 1e-4):\n";
    for(double test_eps : {1e-1, 1e-2, 1e-3, 1e-4, 1e-5}) {
        // Keeping rmin=1e-3, rmax=8.0, acc=1e-4 constant
        auto eq = [&](pp::vector vE) -> pp::vector { return {M(vE[0], 1e-3, 8.0, 1e-4, test_eps)}; };
        std::cout << "  eps = " << test_eps << " -> E0 = " << pp::newton(eq, {-1.0}, 1e-4)[0] << "\n";
    }

    return 0;
}
