#include "complex.h"

complex::complex() {
    real = 0;
    imag = 0;
}

complex::complex(double r, double i) {
    real = r;
    imag = i;
}

complex complex::plus(const complex& other) const {
    return complex(real + other.real, imag + other.imag);
}

complex complex::minus(const complex& other) const {
    return complex(real - other.real, imag - other.imag);
}

complex complex::multiply(double val) const {
    return complex(real * val, imag * val);
}

double complex::abs() const {
    return std::sqrt(real * real + imag * imag);
}

void complex::print() const {
    std::cout << real << " + " << imag << "i";
}
