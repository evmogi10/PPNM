#include<iostream>
#include<string>
#include<functional>
#include<complex>

template <typename T>
class vec {
public:
    T x, y, z;

    vec(T x_val = {}, T y_val = {}, T z_val = {})
        : x(x_val), y(y_val), z(z_val) {}
    vec<T> operator+(const vec<T>& other) const {
        return vec<T>(x + other.x, y + other.y, z + other.z);
    }
    vec<T> operator*(const T& scalar) const {
        return vec<T>(x * scalar, y * scalar, z * scalar);
    }

    void print() const { std::cout << "[" << x << ", " << y << ", " << z << "]" << std::endl; }
};

int main() {

    // 1. Using Doubles
    vec<double> v1(1.1, 2.2, 3.3);
    vec<double> v2(0.9, 0.8, 0.7);
    vec<double> res_d = v1 + v2;
    std::cout << "Double addition: "; res_d.print();

    // 2. Using Complex Numbers
    using namespace std::complex_literals;
    vec<std::complex<double>> vc1({1, 2}, {3, 4}, {5, 6});
    vec<std::complex<double>> vc2 = vc1 * std::complex<double>(2, 0);
    std::cout << "Complex multiplication: "; vc2.print();

return 0;
}
