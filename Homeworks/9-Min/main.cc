/* Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .*/
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "minimize.h"

int main() {
    std::cout << "--- Parts A & C: Minimization Testing ---\n\n";

    // 1. Rosenbrock's Valley Function
    auto rosenbrock = [](pp::vector v) {
        double x = v[0], y = v[1];
        return std::pow(1.0 - x, 2) + 100.0 * std::pow(y - x * x, 2);
    };

    pp::vector rb_start = {-1.0, 2.0};
    auto [rb_fwd, step_rb_fwd] = pp::newton_min(rosenbrock, rb_start, 1e-4, false);
    auto [rb_cen, step_rb_cen] = pp::newton_min(rosenbrock, rb_start, 1e-4, true);

    std::cout << "Rosenbrock Minima (Expected: 1, 1):\n";
    std::cout << "  Forward Difference : "; rb_fwd.print(""); std::cout << "    (Steps: " << step_rb_fwd << ")\n";
    std::cout << "  Central Difference : "; rb_cen.print(""); std::cout << "    (Steps: " << step_rb_cen << ")\n\n";

    // 2. Himmelblau's Function
    auto himmelblau = [](pp::vector v) {
        double x = v[0], y = v[1];
        return std::pow(x * x + y - 11.0, 2) + std::pow(x + y * y - 7.0, 2);
    };

    std::cout << "Himmelblau Minima:\n";
    pp::vector hb_starts[] = {{3, 2}, {-2.5, 3}, {-3, -3}, {2.5, -2}};
    for (auto start : hb_starts) {
        auto [hb_fwd, step_hb_fwd] = pp::newton_min(himmelblau, start, 1e-4, false);
        auto [hb_cen, step_hb_cen] = pp::newton_min(himmelblau, start, 1e-4, true);
        
        std::cout << "  Starting Point: (" << start[0] << ", " << start[1] << ")\n";
        std::cout << "    Forward Steps: " << step_hb_fwd << " -> "; hb_fwd.print("");
        std::cout << "    Central Steps: " << step_hb_cen << " -> "; hb_cen.print("");
    }

    std::cout << "\n--- Part B: Higgs Boson Discovery Fit ---\n\n";

    std::vector<double> energy, signal, error;
    double E, sig, err;
    
    // Read formatted data from stdin
    while (std::cin >> E >> sig >> err) {
        energy.push_back(E);
        signal.push_back(sig);
        error.push_back(err);
    }

    if (energy.empty()) {
        std::cerr << "Warning: No data provided via stdin. Skipping Part B.\n";
        return 1;
    }

    // Deviation function D(m, Gamma, A)
    auto D = [&](pp::vector params) {
        double m = params[0];
        double Gamma = params[1];
        double A = params[2];
        double sum = 0;
        for (size_t i = 0; i < energy.size(); i++) {
            double F = A / (std::pow(energy[i] - m, 2) + std::pow(Gamma, 2) / 4.0);
            double diff = (F - signal[i]) / error[i];
            sum += diff * diff;
        }
        return sum;
    };

    // Initial Guess based on visual peak: Mass ~125, Gamma ~2, Scale ~10
    pp::vector higgs_start = {126.0, 2.0, 10.0};
    
    // Central differences are mathematically cleaner for physics fits
    auto [higgs_res, higgs_steps] = pp::newton_min(D, higgs_start, 1e-4, true);

    std::cout << "Fitted Breit-Wigner Parameters:\n";
    std::cout << "  Mass (m)  = " << higgs_res[0] << " GeV/c^2\n";
    std::cout << "  Width (Γ) = " << higgs_res[1] << " GeV/c^2\n";
    std::cout << "  Scale (A) = " << higgs_res[2] << "\n";
    std::cout << "  Converged in " << higgs_steps << " steps.\n";

    // Export the continuous function line for Gnuplot
    std::ofstream fit_file("higgs_fit.txt");
    for (double e = energy.front(); e <= energy.back(); e += 0.25) {
        double F = higgs_res[2] / (std::pow(e - higgs_res[0], 2) + std::pow(higgs_res[1], 2) / 4.0);
        fit_file << e << " " << F << "\n";
    }
    fit_file.close();

    return 0;
}

/*
 * Comparison Analysis: Forward vs. Central Differences
 * ---------------------------------------------------
 * Both methods yielded identical step counts (21, 1, 5, 6, 5) for the
 * analytical test functions, showing that forward differences are
 * sufficient for smooth landscapes. However, the Central Difference
 * implementation is theoretically superior, offering $O(\delta x^2)$
 * accuracy versus $O(\delta x)$ for forward differences.
 *
 * By sharing function evaluations between the gradient and Hessian, the
 * central approach also maximizes computational efficiency and numerical
 * stability, making it significantly more robust for noisy experimental
 * data like the Higgs boson dataset.
 */
