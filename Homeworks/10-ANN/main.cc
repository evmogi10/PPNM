/* Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .*/
#include "matrix.h"
#include "minimize.h"
#include "ann.h"
#include <iostream>
#include <cmath>
#include <fstream>

int main() {
    std::cout << "Training ANN for function interpolation...\n";

    // Target function: g(x) = cos(5x - 1) * exp(-x^2)
    auto g = [](double x) { return std::cos(5.0 * x - 1.0) * std::exp(-x * x); };

    int N = 40; // More points give the Newton method a clearer map of the waves
    pp::vector x_train(N), y_train(N);
    for (int i = 0; i < N; i++) {
        x_train[i] = -1.0 + 2.0 * i / (N - 1);
        y_train[i] = g(x_train[i]);
    }

    pp::ann net(9); // 9 neurons is the sweet spot for this specific wavelet
    net.train(x_train, y_train);

    // Output interpolation results
    std::ofstream out_interp("interpolation.txt");
    out_interp << "# x   y_true   y_ann   dy_ann   d2y_ann   int_ann\n";
    for (double x = -1.0; x <= 1.0; x += 0.02) {
        out_interp << x << " "
                   << g(x) << " "
                   << net.response(x) << " "
                   << net.derivative(x) << " "
                   << net.derivative2(x) << " "
                   << net.antiderivative(x) << "\n";
    }
    out_interp.close();
    std::cout << "-> Interpolation data saved to 'interpolation.txt'.\n\n";

    std::cout << "Training ANN to solve the ODE: y'' + y = 0...\n";

    auto phi = [](double d2y, double dy, double y, double x) {
        (void)dy; (void)x;
        return d2y + y;
    };

    pp::ann net_ode(6);
    double c = 0.0, yc = 1.0, dyc = 0.0;
    double alpha = 100.0, beta = 100.0;

    net_ode.train_ode(phi, -1.0, 1.0, c, yc, dyc, alpha, beta, 30);

    // Output ODE results
    std::ofstream out_ode("ode_solution.txt");
    out_ode << "# x   y_exact   y_ann\n";
    for (double x = -1.0; x <= 1.0; x += 0.05) {
        out_ode << x << " " << std::cos(x) << " " << net_ode.response(x) << "\n";
    }
    out_ode.close();
    std::cout << "-> ODE Solver data saved to 'ode_solution.txt'.\n";

    return 0;
}
