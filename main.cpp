#include "fractalParser.h"
#include "fractal.h"
#include "generalException.h"
#include "fractalDrawer.h"
#include "complex.h"
#include "point2d.h"
#include "math.h"
#include <memory>
#include "bmp.h"
#include "IFractalRenderer.h"
#include "SimpleCpuMandelbrotJuliaRenderer.h"
#include "JuliaSetAroundMandelbrotRenderer.h"

ColorPaletteProvider* colorPaletteProvider;
MandelbrotJuliaImageGenerator* mandelbrotJuliaImageGenerator;
FractalParser* parser;
FractalDrawer* fractalDrawer;
std::vector<IFractalRenderer*> fractalRenderers;

void showUsage() {
	std::cout << "Fractal drawing program in C++\nUsage:\n./programName fileToParse.frak\nFor example:\n./a.out \"./fractal_definitions/red_outline_mandelbrot.frak\"" << std::endl;
}

void wireUpServices() {
	colorPaletteProvider = new ColorPaletteProvider();
	mandelbrotJuliaImageGenerator = new MandelbrotJuliaImageGenerator(colorPaletteProvider);
	parser = new FractalParser();
	fractalDrawer = new FractalDrawer();

	SimpleCpuMandelbrotJuliaRenderer* simpleCpuMandelbrotJuliaRenderer = new SimpleCpuMandelbrotJuliaRenderer(mandelbrotJuliaImageGenerator, fractalDrawer);
	JuliaSetAroundMandelbrotRenderer* juliaSetAroundMandelbrotRenderer = new JuliaSetAroundMandelbrotRenderer(simpleCpuMandelbrotJuliaRenderer, mandelbrotJuliaImageGenerator, fractalDrawer);
	fractalRenderers.push_back(simpleCpuMandelbrotJuliaRenderer);
	fractalRenderers.push_back(juliaSetAroundMandelbrotRenderer);
}

void cleanupServices() {
	for (IFractalRenderer* renderer : fractalRenderers) {
		delete renderer;
	}
	delete parser;
	delete fractalDrawer;
	delete mandelbrotJuliaImageGenerator;
	delete colorPaletteProvider;
}

int main(int argc, char** argv) {
	if (argc <= 1) {
		showUsage();
		return 0;
	}
	wireUpServices();
	std::map<std::string, std::string> arguments;
	try {
		FractalParams params = parser->readFractal(argv[1]);
		for (IFractalRenderer* renderer : fractalRenderers) {
			if (renderer->isSupported(params.type, arguments)) {
				renderer->render(params, arguments);
				break;
			}
		}
	} catch(const GeneralException& ex) {
		std::cout << ex.what() << std::endl;
	}
	cleanupServices();
}
