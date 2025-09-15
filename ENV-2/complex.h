#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include <cmath>

class complex {
private:
    double real;
    double imag;
public:
    complex();
    complex(double r, double i);
    complex plus(const complex& other) const;
    complex minus(const complex& other) const;
    complex multiply(double val) const;
    double abs() const;
    void print() const;
};

#endif
