#ifndef FRACTAL_DRAWER
#define FRACTAL_DRAWER
#include <memory>
#include "complex.h"

class Bmp;

class FractalDrawer {
public:
	void draw(const std::shared_ptr<Bmp> bmp, const std::string& fileName);
};

#endif