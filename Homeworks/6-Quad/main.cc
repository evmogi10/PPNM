/* Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .*/
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include "integrate.h"

// Global evaluation counter
int evals = 0;

// Wrapper to count evaluations
template<typename F>
auto count_evals(F func) {
    return [func](double x) {
        evals++;
        return func(x);
    };
}

// Implementation of the Error Function
double my_erf(double z) {
    if (z < 0) return -my_erf(-z);

    if (z <= 1.0) {
        auto func = [](double x) { return std::exp(-x * x); };
        return 2.0 / std::sqrt(M_PI) * pp::integrate(func, 0.0, z).value;
    } else {
        auto func = [&](double t) {
            double pt = z + (1.0 - t) / t;
            return std::exp(-pt * pt) / (t * t);
        };
        return 1.0 - 2.0 / std::sqrt(M_PI) * pp::integrate(func, 0.0, 1.0).value;
    }
}

int main() {
    std::cout << "--- Part 1: Basic Integrals ---\n";

    auto f1 = count_evals([](double x) { return std::sqrt(x); });
    evals = 0; auto res1 = pp::integrate(f1, 0.0, 1.0);
    std::cout << "Integral sqrt(x) dx [0,1] = " << res1.value << " (Exact: 2/3), Evals: " << evals << ", Est. Error: " << res1.error << "\n";

    auto f2 = count_evals([](double x) { return 1.0 / std::sqrt(x); });
    evals = 0; auto res2 = pp::integrate(f2, 0.0, 1.0);
    std::cout << "Integral 1/sqrt(x) dx [0,1] = " << res2.value << " (Exact: 2), Evals: " << evals << ", Est. Error: " << res2.error << "\n";

    auto f3 = count_evals([](double x) { return std::log(x) / std::sqrt(x); });
    evals = 0; auto res3 = pp::integrate(f3, 0.0, 1.0);
    std::cout << "Integral ln(x)/sqrt(x) dx [0,1] = " << res3.value << " (Exact: -4), Evals: " << evals << ", Est. Error: " << res3.error << "\n\n";

    std::cout << "--- Part 2: Error Function (erf) ---\n";
    std::ofstream erf_file("erf_plot.txt");
    for (double z = -3.0; z <= 3.0; z += 0.1) {
        erf_file << z << " " << my_erf(z) << " " << std::erf(z) << "\n";
    }
    erf_file.close();

    double exact_erf1 = 0.84270079294971486934;
    std::ofstream conv_file("erf_conv.txt");
    for (double acc = 0.1; acc >= 1e-8; acc /= 10.0) {
        auto func = [](double x) { return std::exp(-x * x); };
        double computed = 2.0 / std::sqrt(M_PI) * pp::integrate(func, 0.0, 1.0, acc, 0.0).value;
        double diff = std::abs(computed - exact_erf1);
        conv_file << acc << " " << diff << "\n";
    }
    conv_file.close();
    std::cout << "erf(1) tested with decreasing acc. Data written to erf_conv.txt for log-log plot.\n\n";

    std::cout << "--- Part 3: Clenshaw-Curtis vs Ordinary ---\n";
    auto f_div1 = [](double x) { return 1.0 / std::sqrt(x); };

    evals = 0; pp::integrate(count_evals(f_div1), 0.0, 1.0); int eval_ord1 = evals;
    evals = 0; pp::integrate_cc(count_evals(f_div1), 0.0, 1.0); int eval_cc1 = evals;
    std::cout << "Integral 1/sqrt(x) [0,1]: Ordinary Evals = " << eval_ord1 << ", CC Evals = " << eval_cc1 << "\n";

    auto f_div2 = [](double x) { return std::log(x) / std::sqrt(x); };
    evals = 0; pp::integrate(count_evals(f_div2), 0.0, 1.0); int eval_ord2 = evals;
    evals = 0; pp::integrate_cc(count_evals(f_div2), 0.0, 1.0); int eval_cc2 = evals;
    std::cout << "Integral ln(x)/sqrt(x) [0,1]: Ordinary Evals = " << eval_ord2 << ", CC Evals = " << eval_cc2 << "\n\n";

    std::cout << "--- Part 4: Infinite Limits ---\n";
    auto f_inf1 = [](double x) { return std::exp(-x); };
    evals = 0; auto res_inf1 = pp::integrate_inf(count_evals(f_inf1), 0.0);
    std::cout << "Integral exp(-x) dx [0, inf] = " << res_inf1.value << " (Exact: 1), Evals: " << evals << "\n";

    auto f_inf2 = [](double x) { return std::exp(-x * x); };
    evals = 0; auto res_inf2 = pp::integrate_inf_inf(count_evals(f_inf2));
    std::cout << "Integral exp(-x^2) dx [-inf, inf] = " << res_inf2.value << " (Exact: " << std::sqrt(M_PI) << "), Evals: " << evals << "\n";

    return 0;
}
