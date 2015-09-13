#ifndef FRACTAL_PARAMS_H
#define FRACTAL_PARAMS_H
#include <string>
#include "complex.h"
#include "vector.h"
#include <string>
#include <vector>

struct ColorInterpolation {
	Vector startColor;
	Vector endColor;
	double multiplier;
	
	ColorInterpolation(Vector startColor, Vector endColor, double multiplier) {
		this->startColor = startColor;
		this->endColor = endColor;
		this->multiplier = multiplier;
	}
};

struct FractalParams {
	int width;
	int height;
	int iterationLimit;
	double minY, maxY;
	double minX, maxX;
	std::string fileName;
	bool isJuliaSet;
	Complex juliaSetStartParameter;
	std::vector<ColorInterpolation> colors;
};
#endif
