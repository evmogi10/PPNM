/* Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .*/
#pragma once
#include "matrix.h"
#include <tuple>
#include <vector>
#include <functional>
#include <cmath>
#include <algorithm>

namespace pp {

// Task A: Runge-Kutta Euler/Midpoint 12-method
inline std::tuple<pp::vector, pp::vector> rkstep12(
    std::function<pp::vector(double, pp::vector)> f,
    double x, 
    pp::vector y, 
    double h)
{
    pp::vector k0 = f(x, y);
    pp::vector k1 = f(x + h/2.0, y + k0 * (h/2.0));
    pp::vector yh = y + k1 * h;
    pp::vector dy = (k1 - k0) * h;
    
    return std::make_tuple(yh, dy);
}

// Task A: Adaptive-step-size driver
inline std::tuple<std::vector<double>, std::vector<pp::vector>> driver(
    std::function<pp::vector(double, pp::vector)> F,
    double a, double b,
    pp::vector yinit,
    double h = 0.125,
    double acc = 0.01,
    double eps = 0.01)
{
    double x = a;
    pp::vector y = yinit;
    
    std::vector<double> xlist; 
    xlist.push_back(x);
    
    std::vector<pp::vector> ylist; 
    ylist.push_back(y);

    do {
        if(x >= b) return std::make_tuple(xlist, ylist); // Job done
        if(x + h > b) h = b - x; // Last step should end exactly at b
        
        auto [yh, dy] = rkstep12(F, x, y, h);
        
        double tol = (acc + eps * yh.norm()) * std::sqrt(h / (b - a));
        double err = dy.norm();
        
        if (err <= tol) { // Accept step
            x += h; 
            y = yh;
            xlist.push_back(x);
            ylist.push_back(y);
        }
        
        // Readjust stepsize
        if (err > 0) {
            h *= std::min(std::pow(tol / err, 0.25) * 0.95, 2.0);
        } else {
            h *= 2.0;
        }
    } while(true);
}

} // namespace pp
