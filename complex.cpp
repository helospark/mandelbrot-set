#include "complex.h"

Complex::Complex(double real, double imaginary) {
	this->real = real;
	this->imaginary = imaginary;
}

double Complex::lengthSquared() {
	return real * real + imaginary * imaginary;
}

Complex Complex::operator*(const Complex& complex) {
	double newReal = real * complex.real - imaginary * complex.imaginary;
	double newImaginary = real * complex.imaginary + imaginary * complex.real;
	return Complex(newReal, newImaginary);
}

Complex Complex::operator+(const Complex& complex) {
	double newReal = real + complex.real;
	double newImaginary = imaginary + complex.imaginary;
	return Complex(newReal, newImaginary);
}

Complex& Complex::operator=(const Complex& complex) {
	this->real = complex.real;
	this->imaginary = complex.imaginary;
	return *this;
}

Complex Complex::addReal(double real) {
	return Complex(this->real + real, this->imaginary);
}

double Complex::getReal() {
	return real;
}

double Complex::getImaginary() {
	return this->imaginary;
}

void Complex::set(double real, double imaginary) {
	this->real = real;
	this->imaginary = imaginary;
}

std::ostream& operator<<(std::ostream& out, const Complex& complex) {
	out << "(" << complex.real << ", " << "i" << complex.imaginary << ")" << std::endl;
	return out;
}
