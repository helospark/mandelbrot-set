#ifndef FRACTAL_PARAMS_H
#define FRACTAL_PARAMS_H

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

#include "complex.h"
#include "vector.h"

struct ColorInterpolation {
	Vector startColor;
	Vector endColor;
	double multiplier;
	
	ColorInterpolation(Vector startColor, Vector endColor, double multiplier) {
		this->startColor = startColor;
		this->endColor = endColor;
		this->multiplier = multiplier;
	}

	bool operator==(const ColorInterpolation& other) const {
		return this->startColor == other.startColor && this->endColor == other.endColor &&
				this->multiplier == other.multiplier;
	}
};

struct FractalParams {
	std::string type;
	int width;
	int height;
	int iterationLimit;
	double minY, maxY;
	double minX, maxX;
	std::string fileName;
	Complex juliaSetStartParameter;
	Vector maxIterationReachedColor;
	std::vector<ColorInterpolation> colors;

	int outlineMandelbrotWidth;
  int outlineMandelbrotHeight;
};

namespace std {
template <>
  struct hash<ColorInterpolation> {
    size_t operator()(const ColorInterpolation& request) const {
        return hash<Vector>()(request.startColor) ^
        		hash<Vector>()(request.endColor) ^
				hash<double>()(request.multiplier);
    }
  };
}
#endif
