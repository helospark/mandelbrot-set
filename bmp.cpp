#include "bmp.h"
#include "vector.h"
#include <fstream>
#include <iostream>

const int BPP=32;

struct bmpfile_magic {
  unsigned char magic[2];
};
 
struct bmpfile_header {
  int filesz;
  short creator1;
  short creator2;
  int bmp_offset;
};

typedef struct {
  int header_sz;
  int width;
  int height;
  short nplanes;
  short bitspp;
  int compress_type;
  int bmp_bytesz;
  int hres;
  int vres;
  int ncolors;
  int nimpcolors;
} BITMAPINFOHEADER;


Bmp::Bmp(int width, int height) {
	this->width = width;
	this->height = height;
	image = new Vector*[height];
	for (int i = 0; i < height; ++i) {
		image[i] = new Vector[width];
	}
}

Bmp::~Bmp() {
	for (int i = 0; i < height; ++i) {
		delete[] image[i];
	}
	delete[] image;
}

void Bmp::fill(Vector color) {
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			image[i][j] = color;
		}
	}
}

void Bmp::setPixel(int x, int y, Vector color) {
	if (x < 0 || y < 0 || x >= width || y >= height) {
		return;
	}
	image[y][x] = color;
}

void Bmp::save(std::string fileName) {
	std::ofstream out(fileName.c_str(),std::ios::binary);
	bmpfile_magic bm={'B','M'};
	bmpfile_header bh={54+((BPP*width)/8)*height,0,0,54};
	BITMAPINFOHEADER bhi={40,width,height,1,BPP,0,((BPP*width)/8)*height,2750,2750,0,0};
	out.write((char*)&bm,sizeof(bm));
	out.write((char*)&bh,sizeof(bh));
	out.write((char*)&bhi,sizeof(bhi));
	for(int i=0;i < height;i++) {
		for(int j=0;j < width;j++)
		{
				out.put((char)image[i][j].getZ());
				out.put((char)image[i][j].getY());
				out.put((char)image[i][j].getX());
				out.put((char)255);
		}
	}
	out.close();
}
