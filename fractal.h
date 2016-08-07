#ifndef FRACTAL_H
#define FRACTAL_H
#include <memory>
#include "fractalParams.h"
#include "vector.h"
#include "ColorPaletteProvider.h"

class Bmp;

class MandelbrotJuliaImageGenerator {
	ColorPaletteProvider* colorPaletteProvider;
	double map(int value, int originalRangeMin, int originalRangeMax, double newRangeMin, double newRangeMax);
	void fillPalette();
	bool isSurelyPartOfMandelbrot(const Complex& c);
	void doJob(const std::shared_ptr<Bmp> image, const FractalParams& params, std::shared_ptr<Vector> palette, unsigned int threadOffset);

	public:
		MandelbrotJuliaImageGenerator(ColorPaletteProvider* colorPaletteProvider);
		std::shared_ptr<Bmp> generateImage(const FractalParams& params);
};

#endif
