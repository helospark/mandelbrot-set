#ifndef BMP_H
#define BMP_H
#include "vector.h"
#include "point2d.h"
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
		void setPixel(Point2d point, Vector color) {
			setPixel(point.x, point.y, color);
		}
		void save(std::string fileName);
		
		int getWidth() {
			return width;
		}
		
		int getHeight() {
			return height;
		}
		
		Vector& getPixel(const Point2d& point) {
			return image[point.y][point.x];
		}
};
#endif
