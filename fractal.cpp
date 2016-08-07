#include "fractal.h"
#include "bmp.h"
#include "math.h"
#include <memory>
#include <ctime>
#include <string>
#include <sstream>
#include <cmath>
#include <thread>

const int NUM_THREADS = 4;

MandelbrotJuliaImageGenerator::MandelbrotJuliaImageGenerator(ColorPaletteProvider* colorPaletteProvider) {
	this->colorPaletteProvider = colorPaletteProvider;
}

bool MandelbrotJuliaImageGenerator::isSurelyPartOfMandelbrot(const Complex& c) {
	bool partOfMandelbrot = false;
	double q = (c.real - 1.0 / 4) * (c.real - 1.0 / 4) + c.imaginary * c.imaginary;
	partOfMandelbrot = (q * (q + (c.real - 1.0 / 4)) < 1.0 / 4 * c.imaginary * c.imaginary);
	partOfMandelbrot = partOfMandelbrot || ((c.real + 1) * (c.real + 1) + c.imaginary * c.imaginary < 1.0 / 16);
	return partOfMandelbrot;
}

void MandelbrotJuliaImageGenerator::doJob(const std::shared_ptr<Bmp> image, const FractalParams& params, std::shared_ptr<Vector> palette, unsigned int threadOffset) {
	for (int i = threadOffset; i < params.height; i += NUM_THREADS) {
		for (int j = 0; j < params.width; ++j) {
			double real      = Math::map(j, 0, params.width, params.minX, params.maxX);
			double imaginary = Math::map(i, 0, params.height, params.minY, params.maxY);
			Complex c(real, imaginary);
			Complex z(0,0);
			if (params.type != "julia" && isSurelyPartOfMandelbrot(c)) {
				continue;
			}
			if (params.type == "julia") {
				z.set(real, imaginary);
				c.set(params.juliaSetStartParameter);
			}
			
			int k = 0;
			while (k < params.iterationLimit && z.lengthSquared() <  4.0) {
				z = z * z + c;
				++k;
			}
			image->setPixel(j, i, palette.get()[k]);
		}
		//if (i % 20 == 0)
		//	std::cout << (double)i / params.height * 100.0 << " %" << std::endl;
	}
}

std::shared_ptr<Bmp> MandelbrotJuliaImageGenerator::generateImage(const FractalParams& params) {
	std::shared_ptr<Bmp> image = std::make_shared<Bmp>(params.width, params.height);
	image->fill(Vector(0, 0, 0));
	
	std::thread threads[NUM_THREADS];
	std::shared_ptr<Vector> colorPalette = colorPaletteProvider->provideColorPalette(ColorPaletteRequest(params.colors, params.maxIterationReachedColor, params.iterationLimit));
	
	for (int i = 0; i < NUM_THREADS; ++i) {
		threads[i] = std::thread(&MandelbrotJuliaImageGenerator::doJob, this, image, params, colorPalette, i);
	}
	for (int i = 0; i < NUM_THREADS; ++i) {
		threads[i].join();
	}
	
	return image;
}
