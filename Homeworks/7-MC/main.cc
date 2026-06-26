/* Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .*/
#include <iostream>
#include <fstream>
#include <cmath>
#include "montecarlo.h"

int main() {
    std::cout << "--- Part 1: Plain Monte Carlo ---\n";
    pp::lcg my_lcg;

    // Unit Circle Area
    auto f_circle = [](const std::vector<double>& x) {
        return (x[0]*x[0] + x[1]*x[1] <= 1.0) ? 1.0 : 0.0;
    };
    int N_circle = 100000;
    pp::MCResult res_circ = pp::plainmc(f_circle, {-1, -1}, {1, 1}, N_circle, my_lcg);
    std::cout << "Unit Circle Area = " << res_circ.value << " +/- " << res_circ.error
              << " (Exact: " << M_PI << ")\n";

    // Ellipsoid Volume (a=1, b=2, c=3)
    auto f_ellipsoid = [](const std::vector<double>& x) {
        return (x[0]*x[0]/1.0 + x[1]*x[1]/4.0 + x[2]*x[2]/9.0 <= 1.0) ? 1.0 : 0.0;
    };
    int N_ell = 1000000;
    pp::MCResult res_ell = pp::plainmc(f_ellipsoid, {-1, -2, -3}, {1, 2, 3}, N_ell, my_lcg);
    double exact_ell = (4.0/3.0) * M_PI * 1 * 2 * 3;
    std::cout << "Ellipsoid Volume = " << res_ell.value << " +/- " << res_ell.error
              << " (Exact: " << exact_ell << ")\n\n";

    std::cout << "--- Part 2 & 3: Quasi-Random & Stratified vs Plain ---\n";
    // We use a less singular difficult integral to test scaling:
    // Integral of sin(x)*sin(y)*sin(z) from 0 to PI for all = 8.0
    auto f_smooth = [](const std::vector<double>& x) {
        return std::sin(x[0]) * std::sin(x[1]) * std::sin(x[2]);
    };
    double exact_smooth = 8.0;

    std::ofstream scale_file("mc_scaling.txt");
    std::mt19937 std_rng(42); // C++ standard generator
    auto std_rand_double = [&]() { return std::generate_canonical<double, 10>(std_rng); };

    auto f_hard = [](const std::vector<double>& x) {
        double den = 1.0 - std::cos(x[0]) * std::cos(x[1]) * std::cos(x[2]);
        if (den == 0.0) return 0.0; // Evitar infinito en el origen
        return 1.0 / (M_PI * M_PI * M_PI * den); // Dividimos por PI^3 por los dx/PI del enunciado
    };

    int N_hard = 1000000;
    double exact_hard = 1.393203929685676859;

    std::cout << "\n--- Difficult Singular Integral ---\n";
    std::cout << "Exact value: " << exact_hard << "\n";

    pp::MCResult res_h_lcg = pp::plainmc(f_hard, {0,0,0}, {M_PI,M_PI,M_PI}, N_hard, my_lcg);
    std::cout << "i) LCG:      " << res_h_lcg.value << " (Error: " << std::abs(res_h_lcg.value - exact_hard) << ")\n";

    pp::MCResult res_h_std = pp::plainmc(f_hard, {0,0,0}, {M_PI,M_PI,M_PI}, N_hard, std_rand_double);
    std::cout << "ii) Std C++: " << res_h_std.value << " (Error: " << std::abs(res_h_std.value - exact_hard) << ")\n";

    pp::MCResult res_h_qsi = pp::quasimc(f_hard, {0,0,0}, {M_PI,M_PI,M_PI}, N_hard);
    std::cout << "iii) Quasi:  " << res_h_qsi.value << " (Error: " << std::abs(res_h_qsi.value - exact_hard) << ")\n\n";


    std::cout << "Generating scaling data. See plot for 1/sqrt(N) convergence check.\n";
    for (int N = 1000; N <= 1000000; N *= 2) {
        // Plain with LCG
        pp::MCResult res_plain = pp::plainmc(f_smooth, {0,0,0}, {M_PI,M_PI,M_PI}, N, my_lcg);
        double err_plain = std::abs(res_plain.value - exact_smooth);

        // Quasi
        pp::MCResult res_quasi = pp::quasimc(f_smooth, {0,0,0}, {M_PI,M_PI,M_PI}, N);
        double err_quasi = std::abs(res_quasi.value - exact_smooth);

        // Stratified
        pp::MCResult res_strat = pp::stratmc(f_smooth, {0,0,0}, {M_PI,M_PI,M_PI}, N, my_lcg);
        double err_strat = std::abs(res_strat.value - exact_smooth);

        // N, err_plain, err_quasi, err_strat, reference 1/sqrt(N)
        scale_file << N << " " << err_plain << " " << err_quasi << " "
                   << err_strat << " " << 10.0 / std::sqrt(N) << "\n";
    }
    scale_file.close();
    std::cout << "Data saved to mc_scaling.txt\n";

    return 0;
}
