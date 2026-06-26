/* Gemini was used to assist with the implementation and code generation, which I then reviewed, adapted, and tested .*/
#include "matrix.h"
#include "ode.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

int main() {
    // -----------------------------------------------------------------
    // Task A: Harmonic Oscillator (u'' = -u => y0' = y1, y1' = -y0)
    // -----------------------------------------------------------------
    auto f_osc = [](double /*x*/, pp::vector y) {
        return pp::vector{y[1], -y[0]};
    };
    
    auto [x_osc, y_osc] = pp::driver(f_osc, 0, 10, {1.0, 0.0}, 0.1, 1e-4, 1e-4);
    
    std::ofstream out_osc("oscillator.txt");
    for (size_t i = 0; i < x_osc.size(); ++i) {
        out_osc << x_osc[i] << " " << y_osc[i][0] << " " << y_osc[i][1] << "\n";
    }
    out_osc.close();

    // -----------------------------------------------------------------
    // Task B: Relativistic Planetary Orbit 
    // u'' + u = 1 + eps*u^2 => y0' = y1, y1' = 1 - y0 + eps*y0^2
    // -----------------------------------------------------------------
    auto solve_orbit = [](double eps, pp::vector yinit, std::string filename) {
        auto f_orbit = [eps](double /*phi*/, pp::vector y) {
            return pp::vector{y[1], 1.0 - y[0] + eps * y[0] * y[0]};
        };
        
        // Integrate for several rotations (e.g., 20 pi)
        auto [phi, u_vec] = pp::driver(f_orbit, 0, 20 * M_PI, yinit, 0.1, 1e-6, 1e-6);
        
        std::ofstream out(filename);
        for (size_t i = 0; i < phi.size(); ++i) {
            out << phi[i] << " " << u_vec[i][0] << "\n";
        }
        out.close();
    };

    solve_orbit(0.0,  {1.0, 0.0},  "orbit_circ.txt");     // 1. Circular (eps=0)
    solve_orbit(0.0,  {1.0, -0.5}, "orbit_ellip.txt");    // 2. Elliptical (eps=0)
    solve_orbit(0.01, {1.0, -0.5}, "orbit_precess.txt");  // 3. Relativistic Precession

    // -----------------------------------------------------------------
    // Task C: Figure-8 Three-Body Problem
    // -----------------------------------------------------------------
    auto f_3body = [](double /*t*/, pp::vector z) {
        // Extract positions
        double r1x = z[6], r1y = z[7];
        double r2x = z[8], r2y = z[9];
        double r3x = z[10], r3y = z[11];
        
        // Distances cubed: |ri - rj|^3
        double dx12 = r2x - r1x, dy12 = r2y - r1y;
        double d12 = std::pow(dx12*dx12 + dy12*dy12, 1.5);
        
        double dx13 = r3x - r1x, dy13 = r3y - r1y;
        double d13 = std::pow(dx13*dx13 + dy13*dy13, 1.5);
        
        double dx23 = r3x - r2x, dy23 = r3y - r2y;
        double d23 = std::pow(dx23*dx23 + dy23*dy23, 1.5);

        // Accelerations (Gravity forces)
        double a1x = dx12/d12 + dx13/d13;
        double a1y = dy12/d12 + dy13/d13;
        
        double a2x = -dx12/d12 + dx23/d23;
        double a2y = -dy12/d12 + dy23/d23;
        
        double a3x = -dx13/d13 - dx23/d23;
        double a3y = -dy13/d13 - dy23/d23;

        // Return {v1', v2', v3', r1', r2', r3'}
        return pp::vector{a1x, a1y, a2x, a2y, a3x, a3y, 
                          z[0], z[1], z[2], z[3], z[4], z[5]};
    };

    // Wikipedia Initial Conditions
    pp::vector zinit = {
        0.466203685,  0.43236573,    // v1
        0.466203685,  0.43236573,    // v2
       -0.93240737,  -0.86473146,    // v3
        0.97000436,  -0.24308753,    // r1
       -0.97000436,   0.24308753,    // r2
        0.0,          0.0            // r3
    };

    double T = 6.3259; // One full period
    auto [t, z] = pp::driver(f_3body, 0, T, zinit, 0.01, 1e-6, 1e-6); // High precision for chaos stability

    std::ofstream out_3body("figure8.txt");
    for (size_t i = 0; i < t.size(); i += 10) {
        out_3body << z[i][6] << " " << z[i][7] << " "     // x1, y1
                  << z[i][8] << " " << z[i][9] << " "     // x2, y2
                  << z[i][10] << " " << z[i][11] << "\n"; // x3, y3
    }
    out_3body.close();

    std::cout << "Data files generated successfully.\n";
    return 0;
}
