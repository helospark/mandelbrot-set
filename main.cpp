#include "fractalParser.h"
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
#include "ThreadedMandelbrotJuliaImageGenerator.h"
#include "argumentParser.h"

ColorPaletteProvider* colorPaletteProvider;
ThreadedMandelbrotJuliaImageGenerator* mandelbrotJuliaImageGenerator;
FractalParser* parser;
FractalDrawer* fractalDrawer;
std::vector<IFractalRenderer*> fractalRenderers;
ArgumentParser argumentParser;

void showUsage(const char* message = nullptr) {
  if (message != nullptr) {
    std::cout << message << '\n';
  }
	std::cout << "Fractal drawing program in C++\nUsage:\n./programName fileToParse.frak\nFor example:\n./a.out \"./fractal_definitions/red_outline_mandelbrot.frak\"" << std::endl;
}

std::vector<Argument> createArguments() {
  std::vector<Argument> arguments;
  arguments.push_back(Argument('h', "help", "Display this help", BOOLEAN_ARGUMENT));
  arguments.push_back(Argument('f', "filename", "Name of the file to render, mandatory argument", STRING_ARGUMENT));
  arguments.push_back(Argument('t', "threads", "Number of threads, defaults to core count", INTEGER_ARGUMENT));
  arguments.push_back(Argument('s', "startIndex", "For all animations the index to start from", INTEGER_ARGUMENT));
  return arguments;
}

void wireUpServices() {
	colorPaletteProvider = new ColorPaletteProvider();
	mandelbrotJuliaImageGenerator = new ThreadedMandelbrotJuliaImageGenerator(colorPaletteProvider);
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
  try {
    wireUpServices();
    argumentParser.parse(argc, argv, createArguments());

    if (argumentParser.isArgumentPresent('h')) {
      showUsage();
      return 0;
    }
    if (!argumentParser.isArgumentPresent('f')) {
      showUsage("No filename is present, give with -f");
      return -1;
    }
    std::string fileName = argumentParser.getStringArgument('f');

    std::map<std::string, std::string> arguments = argumentParser.getRawMap();
    FractalParams params = parser->readFractal(fileName);
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
