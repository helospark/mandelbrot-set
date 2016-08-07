#include "complex.h"
#include "generalException.h"
#include <cstring>
#include <algorithm>

Complex::Complex() {
	this->real = 0.0;
	this->imaginary = 0.0;
}

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

double Complex::getReal() const {
	return real;
}

double Complex::getImaginary() const {
	return this->imaginary;
}

Complex& Complex::set(double real, double imaginary) {
	this->real = real;
	this->imaginary = imaginary;
	return *this;
}

Complex& Complex::set(const Complex& complex) {
	this->real = complex.real;
	this->imaginary = complex.imaginary;
	return *this;
}
		

Complex& Complex::parseFromString(const std::string& data) {
	std::string localData = data;
	const char* charsToRemove = " \t()";
  for (unsigned int i = 0; i < std::strlen(charsToRemove); ++i) {
    localData.erase(std::remove(localData.begin(), localData.end(), charsToRemove[i]), localData.end());
  }
  size_t separator = localData.find(",");
  if (separator == std::string::npos) {
  	throw new GeneralException("Cannot parse complex data, missing ',' separator");
  }
  this->real = atof(localData.substr(0, separator).c_str());
  this->imaginary = atof(localData.substr(separator + 1, std::string::npos).c_str());
  return *this;
}

std::ostream& operator<<(std::ostream& out, const Complex& complex) {
	out << "(" << complex.real << ", " << "i" << complex.imaginary << ")";
	return out;
}
