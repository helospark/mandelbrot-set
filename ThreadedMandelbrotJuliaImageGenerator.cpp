#include "bmp.h"
#include "math.h"
#include <memory>
#include <ctime>
#include <string>
#include <sstream>
#include <cmath>
#include <thread>
#include "FractalType.h"
#include "ThreadedMandelbrotJuliaImageGenerator.h"

ThreadedMandelbrotJuliaImageGenerator::ThreadedMandelbrotJuliaImageGenerator(ColorPaletteProvider* colorPaletteProvider) {
	this->colorPaletteProvider = colorPaletteProvider;
}

bool ThreadedMandelbrotJuliaImageGenerator::isSurelyPartOfMandelbrot(const Complex& c) {
	bool partOfMandelbrot = false;
	double q = (c.real - 1.0 / 4) * (c.real - 1.0 / 4) + c.imaginary * c.imaginary;
	partOfMandelbrot = (q * (q + (c.real - 1.0 / 4)) < 1.0 / 4 * c.imaginary * c.imaginary);
	partOfMandelbrot = partOfMandelbrot || ((c.real + 1) * (c.real + 1) + c.imaginary * c.imaginary < 1.0 / 16);
	return partOfMandelbrot;
}

void ThreadedMandelbrotJuliaImageGenerator::doJob(const std::shared_ptr<Bmp> image, const FractalParams& params, std::shared_ptr<Vector> palette, unsigned int threadOffset, int numberOfThreads) {
	for (int i = threadOffset; i < params.height; i += numberOfThreads) {
		for (int j = 0; j < params.width; ++j) {
			double real      = Math::map(j, 0, params.width, params.minX, params.maxX);
			double imaginary = Math::map(i, 0, params.height, params.minY, params.maxY);
			Complex c(real, imaginary);
			Complex z(0,0);
			if (params.type == FractalType::MANDELBROT && isSurelyPartOfMandelbrot(c)) {
				continue;
			}
			if (params.type == FractalType::JULIA) {
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
	}
}

std::shared_ptr<Bmp> ThreadedMandelbrotJuliaImageGenerator::generateImage(const FractalParams& params, int numberOfThreads) {
  std::shared_ptr<Bmp> image = std::make_shared<Bmp>(params.width, params.height);
	image->fill(params.maxIterationReachedColor);
	
	std::thread* threads = new std::thread[numberOfThreads];
	std::shared_ptr<Vector> colorPalette = colorPaletteProvider->provideColorPalette(ColorPaletteRequest(params.colors, params.maxIterationReachedColor, params.iterationLimit));
	
	for (int i = 0; i < numberOfThreads; ++i) {
		threads[i] = std::thread(&ThreadedMandelbrotJuliaImageGenerator::doJob, this, image, params, colorPalette, i, numberOfThreads);
	}
	for (int i = 0; i < numberOfThreads; ++i) {
		threads[i].join();
	}
	delete[] threads;
	
	return image;
}
