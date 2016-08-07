#ifndef MATH_H
#define MATH_H
#include "Rectangle.h"

namespace Math {
	inline double map(int value, int originalRangeMin, int originalRangeMax, double newRangeMin, double newRangeMax) {
		double mappedToUnitRange = (double)value / (originalRangeMax - originalRangeMin) - originalRangeMin;
		return mappedToUnitRange * (newRangeMax - newRangeMin) + newRangeMin;
	}

	inline Complex interpolate(Complex a, Complex b, double amount) {
		double newReal = a.getReal() * (1.0 - amount) + b.getReal() * amount;
		double newImaginary = a.getImaginary() * (1.0 - amount) + b.getImaginary() * amount;
		return Complex(newReal, newImaginary);
	}

	inline Complex mapComplex(Point2d point, const Rectangle& original, const Rectangle& destination) {
		double real = Math::map(point.x, original.x0, original.x1, destination.x0, destination.x1);
		double imaginary = Math::map(point.y, original.y0, original.y1, destination.y0, destination.y1);
		return Complex(real, imaginary);
	}
};

#endif
