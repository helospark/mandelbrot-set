#include "fractalParser.h"
#include "fractal.h"
#include "generalException.h"

void showUsage() {

	std::cout << "Fractal drawing program in C++\nUsage:\n./programName fileToParse.frak\nFor example:\n./a.out \"./fractal_definitions/red_outline_mandelbrot.frak\"" << std::endl;
}

int main(int argc, char** argv) {
	if (argc <= 1) {
		showUsage();
		return 0;
	}
	try {
		FractalParser parser;
		FractalParams params = parser.readFractal(argv[1]);
		Fractal fractal(params);
		fractal.draw();
	} catch(const GeneralException& ex) {
		std::cout << ex.what() << std::endl;
	}
}
