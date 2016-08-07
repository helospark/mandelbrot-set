#ifndef MY_MATH_H
#define MY_MATH_H
#include "Rectangle.h"
#include <cmath>
#include "point2d.h"
#include "complex.h"
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

	inline bool doubleEquals(double a, double b, double threshold) {
	  return std::abs(a - b) < threshold;
	}

	inline int parseIntOrGet(const std::string& str, int defaultValue) {
	  if (str.size() == 0) {
	    return defaultValue;
	  }
	  int result = 0;
	  int i = 0;
	  bool isNegative = false;
	  if (str[0] == '-') {
	    isNegative = true;
	    ++i;
	  }
	  for (; i < str.size(); ++i) {
	    if (str[i] < '0' || str[i] > '9') {
	      return defaultValue;
	    }
	    result *= 10;
	    result += (str[i] - '0');
	  }
	  if (isNegative) {
	    result *= -1;
	  }
	  return result;
	}
};

#endif
