#include <sstream>
#include <chrono>
#include "fractalDrawer.h"
#include "bmp.h"

void FractalDrawer::draw(const std::shared_ptr<Bmp> bmp, const std::string& fileName) {
	std::stringstream ss;
	auto time = std::chrono::high_resolution_clock::now().time_since_epoch().count();;
	ss << fileName << "_" << time;
	ss << ".bmp";;
	bmp->save(ss.str());
//	std::cout << "Saved fractal to file " << ss.str() << std::endl;
};