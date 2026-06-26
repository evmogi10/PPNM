/* Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .*/
#include <iostream>
#include <fstream>
#include <cmath>
#include "matrix.h"
#include "spline.h"

int main() {
    // --- TASK A: LINEAR SPLINE PLOTS ---
    int n_lin = 10;
    pp::vector x_lin(n_lin), y_lin(n_lin);
    std::ofstream f_data("data.txt");
    
    for(int i = 0; i < n_lin; i++) {
        x_lin[i] = i * 1.0; 
        y_lin[i] = std::cos(x_lin[i]);
        f_data << x_lin[i] << " " << y_lin[i] << "\n";
    }
    f_data.close();

    // --- TASK B: QUADRATIC DEBUGGING HINTS ---
    std::cout << "--- Debugging Quadratic Splines ---\n";
    pp::vector x_q = {1, 2, 3, 4, 5};
    pp::vector y_q1 = {1, 1, 1, 1, 1};         // y = 1
    pp::vector y_q2 = {1, 2, 3, 4, 5};         // y = x
    pp::vector y_q3 = {1, 4, 9, 16, 25};       // y = x^2

    pp::qspline qs1(x_q, y_q1);
    pp::qspline qs2(x_q, y_q2);
    pp::qspline qs3(x_q, y_q3);

    std::cout << "Test y=1   -> qs1(2.5) = " << qs1.eval(2.5) << " (Expected 1)\n";
    std::cout << "Test y=x   -> qs2(2.5) = " << qs2.eval(2.5) << " (Expected 2.5)\n";
    std::cout << "Test y=x^2 -> qs3(2.5) = " << qs3.eval(2.5) << " (Expected 6.25)\n\n";

    // --- GENERATE ALL PLOT DATA ---
    pp::qspline my_qspline(x_lin, y_lin);
    auto functional_qspline = pp::make_qspline_eval(x_lin, y_lin);
    pp::cspline my_cspline(x_lin, y_lin);

    std::ofstream f_plot("plot_curves.txt");
    for(double z = 0.0; z <= 9.0; z += 0.05) {
        double lin_val = pp::linterp(x_lin, y_lin, z);
        double lin_int = pp::linterpInteg(x_lin, y_lin, z);
        
        //double quad_val = my_qspline.eval(z);
	double quad_val = functional_qspline(z);
        double quad_int = my_qspline.integ(z);
        
        double cub_val = my_cspline.eval(z);
        double exact_int = std::sin(z);

        f_plot << z << " " 
               << lin_val << " " << lin_int << " "
               << quad_val << " " << quad_int << " "
               << cub_val << " " << exact_int << "\n";
    }
    f_plot.close();

    return 0;
}
