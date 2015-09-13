#ifndef COMPLEX_H
#define COMPLEX_H
#include <iostream>

class Complex {
	private:
		double real;
		double imaginary;
	public:
		Complex(double real, double imaginary);
		double lengthSquared();
		Complex operator*(const Complex& complex);
		Complex operator+(const Complex& complex);
		Complex& operator=(const Complex& complex);
		friend std::ostream& operator<<(std::ostream& out, const Complex& complex);
		Complex addReal(double real);
		double getReal();
		double getImaginary();
		void set(double real, double imaginary);
};
#endif
