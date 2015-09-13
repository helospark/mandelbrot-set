#ifndef BMP_H
#define BMP_H
#include "vector.h"
#include <string>

class Bmp {
	private:
		int width, height;
		Vector** image;
	public:
		Bmp(int width, int height);
		~Bmp();
		
		void fill(Vector color);
		void setPixel(int x, int y, Vector color);
		void save(std::string fileName);
};
#endif
