#include "fractal.h"
#include "bmp.h"
#include <ctime>
#include <string>
#include <sstream>
#include <cmath>

Fractal::Fractal(const FractalParams& params) {
	this->params = params;
	fillPalette();
}


Fractal::~Fractal() {
	delete[] palette;
}

double Fractal::map(int value, int originalRangeMin, int originalRangeMax, double newRangeMin, double newRangeMax) {
	double mappedToUnitRange = (double)value / (originalRangeMax - originalRangeMin) - originalRangeMin;
	return mappedToUnitRange * (newRangeMax - newRangeMin) + newRangeMin;
}

void Fractal::fillPalette() {
	palette = new Vector[params.iterationLimit + 1];
	palette[params.iterationLimit] = Vector(0, 0, 0);
	
	for (int i = 0; i < params.colors.size(); ++i) {
		int startIndex = (i == 0 ? 0 : ((int)(params.iterationLimit * params.colors[i - 1].multiplier)));
		int endIndex = (int)(params.iterationLimit * params.colors[i].multiplier);
		if (endIndex >= params.iterationLimit) {
			endIndex = params.iterationLimit - 1;
		}
		for (int j = startIndex; j < endIndex; ++j) {
			palette[j] = params.colors[i].startColor.interpolate(params.colors[i].endColor, (double)j / endIndex);
		}
	}
}


void Fractal::draw() {
	Bmp image(params.width, params.height);
	image.fill(Vector(0, 0, 0));
	
	for (int i = 0; i < params.height; ++i) {
		for (int j = 0; j < params.width; ++j) {
			double real      = map(j, 0, params.width, params.minX, params.maxX);
			double imaginary = map(i, 0, params.height, params.minY, params.maxY);
			Complex c(real, imaginary);
			Complex z(0,0);
			if (params.isJuliaSet) {
				z.set(real, imaginary);
				c.set(params.juliaSetStartParameter);
			}
			
			int k = 0;
			while (k < params.iterationLimit && z.lengthSquared() <  4.0) {
				z = z * z + c;
				++k;
			}
			image.setPixel(j, i, palette[k]);
		}
		if (i % 20 == 0)
			std::cout << (double)i / params.height * 100.0 << " %" << std::endl;
	}
	std::stringstream ss;
	ss << params.fileName << "_" << time(NULL) << ".bmp";
	image.save(ss.str());
	std::cout << "Saved fractal to file " << ss.str() << std::endl;
}
