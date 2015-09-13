#ifndef FRACTAL_H
#define FRACTAL_H
#include "fractalParams.h"
#include "vector.h"

class Fractal {
	Vector* palette;
	FractalParams params;
	double map(int value, int originalRangeMin, int originalRangeMax, double newRangeMin, double newRangeMax);
	void fillPalette();

	public:
		Fractal(const FractalParams& params);
		~Fractal();
		void draw();
};

#endif
