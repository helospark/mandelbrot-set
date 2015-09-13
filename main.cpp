#include "complex.h"
#include "vector.h"
#include "bmp.h"
#include <ctime>
#include <string>
#include <sstream>
#include <cmath>

const int WIDTH = 1920 * 6;
const int HEIGHT = 1080 * 6;

const double MIN_X = -2.4;
const double MAX_X = 1.2;
const double MIN_Y = -1.2;
const double MAX_Y = 1.2;

const int ITERATION_LIMIT = 10000;
const std::string fileName = "mandelbrot";

const bool IS_JULIA_SET = false;
const double JULIA_SET_REAL = -0.4;
const double JULIA_SET_IMAGINARY = 0.6;

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

/*
// BLUE
const int COLOR_INTERPOLATION_SIZE = 3;
const double INVERSE_ITERATION_LIMIT_TEST = ITERATION_LIMIT / 1000.0; // tested at this iteration count
const ColorInterpolation colorInterpolations[COLOR_INTERPOLATION_SIZE] = {
	ColorInterpolation(Vector(0, 33, 198), Vector(40, 255, 240), 1.0 / (40.0 * INVERSE_ITERATION_LIMIT_TEST) ),
	ColorInterpolation(Vector(40, 255, 240), Vector(168,255,253), 3.0 / (40.0 * INVERSE_ITERATION_LIMIT_TEST) ),
	ColorInterpolation(Vector(210, 240, 255), Vector(255, 255, 255), 40.0 / (40.0 * INVERSE_ITERATION_LIMIT_TEST) )
};
*/


// FIRE for Mandelbrot set
const int COLOR_INTERPOLATION_SIZE = 8;
const double INVERSE_ITERATION_LIMIT_TEST = ITERATION_LIMIT / 1000.0; // tested at this iteration count
const ColorInterpolation colorInterpolations[COLOR_INTERPOLATION_SIZE] = {
	ColorInterpolation(Vector(53, 0, 0), Vector(225, 17, 17), 0.9 / (60.0 * INVERSE_ITERATION_LIMIT_TEST) ),
	ColorInterpolation(Vector(225, 17, 17), Vector(255, 0, 0), 1.4 / (60.0 * INVERSE_ITERATION_LIMIT_TEST) ),
	ColorInterpolation(Vector(225, 17, 17), Vector(238, 100, 8), 1.5 / (60.0 * INVERSE_ITERATION_LIMIT_TEST) ),
	ColorInterpolation(Vector(238, 100, 8), Vector(236, 240, 0), 1.8 / (60.0 * INVERSE_ITERATION_LIMIT_TEST) ),
	ColorInterpolation(Vector(236, 240, 0), Vector(255, 246, 107), 2.3 / (60.0 * INVERSE_ITERATION_LIMIT_TEST) ),
	ColorInterpolation(Vector(236, 240, 0), Vector(255, 255, 255), 7.0 / (60.0 * INVERSE_ITERATION_LIMIT_TEST) ),
	ColorInterpolation(Vector(255,255,255), Vector(150, 255, 250), 12.0 / (60.0 * INVERSE_ITERATION_LIMIT_TEST) ),
	ColorInterpolation(Vector(150, 255, 250), Vector(150, 255, 250), 60.0 / (60.0 * INVERSE_ITERATION_LIMIT_TEST) )
};
/*
// BLUE galaxies in the sky for Julia set
const int COLOR_INTERPOLATION_SIZE = 4;
const double INVERSE_ITERATION_LIMIT_TEST = ITERATION_LIMIT / 1000.0; // tested at this iteration count
const ColorInterpolation colorInterpolations[COLOR_INTERPOLATION_SIZE] = {
	ColorInterpolation(Vector(0, 0, 0), Vector(0, 0, 0), 1.0 / (80.0 * INVERSE_ITERATION_LIMIT_TEST) ),
	ColorInterpolation(Vector(0, 0, 0), Vector(40, 255, 240), 33.0 / (40.0 * INVERSE_ITERATION_LIMIT_TEST) ),
	ColorInterpolation(Vector(40, 255, 240), Vector(168,255,253), 35.0 / (40.0 * INVERSE_ITERATION_LIMIT_TEST) ),
	ColorInterpolation(Vector(210, 240, 255), Vector(255, 255, 255), 40.0 / (40.0 * INVERSE_ITERATION_LIMIT_TEST) )
}; */
/*
// BLUE light galaxies in the sky for Julia set
const int COLOR_INTERPOLATION_SIZE = 4;
const double INVERSE_ITERATION_LIMIT_TEST = ITERATION_LIMIT / 1000.0; // tested at this iteration count
const ColorInterpolation colorInterpolations[COLOR_INTERPOLATION_SIZE] = {
	ColorInterpolation(Vector(0, 0, 0), Vector(0, 0, 0), 1.0 / (80.0 * INVERSE_ITERATION_LIMIT_TEST) ),
	ColorInterpolation(Vector(0, 0, 0), Vector(40, 255, 240), 10.0 / (40.0 * INVERSE_ITERATION_LIMIT_TEST) ),
	ColorInterpolation(Vector(40, 255, 240), Vector(168,255,253), 35.0 / (40.0 * INVERSE_ITERATION_LIMIT_TEST) ),
	ColorInterpolation(Vector(210, 240, 255), Vector(255, 255, 255), 40.0 / (40.0 * INVERSE_ITERATION_LIMIT_TEST) )
};
*/
/*
// RED outline for Mandelbrot set
const int COLOR_INTERPOLATION_SIZE = 4;
const double INVERSE_ITERATION_LIMIT_TEST = ITERATION_LIMIT / 1000.0; // tested at this iteration count
const ColorInterpolation colorInterpolations[COLOR_INTERPOLATION_SIZE] = {
	ColorInterpolation(Vector(0, 0, 0), Vector(0, 0, 0), 0.2 / (80.0 * INVERSE_ITERATION_LIMIT_TEST) ),
	ColorInterpolation(Vector(0, 0, 0), Vector(128, 0, 0), 2.0 / (40.0 * INVERSE_ITERATION_LIMIT_TEST) ),
	ColorInterpolation(Vector(128, 0, 0), Vector(255,0,0), 10.0 / (40.0 * INVERSE_ITERATION_LIMIT_TEST) ),
	ColorInterpolation(Vector(255, 0, 0), Vector(255, 255, 255), 40.0 / (40.0 * INVERSE_ITERATION_LIMIT_TEST) )
};
*/

Vector* palette;

double map(int value, int originalRangeMin, int originalRangeMax, double newRangeMin, double newRangeMax) {
	double mappedToUnitRange = (double)value / (originalRangeMax - originalRangeMin) - originalRangeMin;
	return mappedToUnitRange * (newRangeMax - newRangeMin) + newRangeMin;
}

void fillPalette() {
	palette = new Vector[ITERATION_LIMIT + 1];
	palette[ITERATION_LIMIT] = Vector(0, 0, 0);
	
	for (int i = 0; i < COLOR_INTERPOLATION_SIZE; ++i) {
		int startIndex = (i == 0 ? 0 : ((int)(ITERATION_LIMIT * colorInterpolations[i - 1].multiplier)));
		int endIndex = (int)(ITERATION_LIMIT * colorInterpolations[i].multiplier);
		if (endIndex >= ITERATION_LIMIT) {
			endIndex = ITERATION_LIMIT - 1;
		}
		for (int j = startIndex; j < endIndex; ++j) {
			palette[j] = colorInterpolations[i].startColor.interpolate(colorInterpolations[i].endColor, (double)j / endIndex);
		}
	}
}

void destroyPalette() {
	delete[] palette;
}

int main() {
	fillPalette();
	Bmp image(WIDTH, HEIGHT);
	image.fill(Vector(0, 0, 0));
	
	for (int i = 0; i < HEIGHT; ++i) {
		for (int j = 0; j < WIDTH; ++j) {
			double real      = map(j, 0, WIDTH, MIN_X, MAX_X);
			double imaginary = map(i, 0, HEIGHT, MIN_Y, MAX_Y);
			Complex c(real, imaginary);
			Complex z(0,0);
			if (IS_JULIA_SET) {
				z.set(real, imaginary);
				c.set(JULIA_SET_REAL, JULIA_SET_IMAGINARY);
			}
			
			int k = 0;
			while (k < ITERATION_LIMIT && z.lengthSquared() <  4.0) {
				z = z * z + c;
				++k;
			}
			image.setPixel(j, i, palette[k]);
		}
		if (i % 20 == 0)
			std::cout << (double)i / HEIGHT * 100.0 << " %" << std::endl;
	}
	std::stringstream ss;
	ss << fileName << "_" << time(NULL) << ".bmp";
	image.save(ss.str());
	destroyPalette();
}
