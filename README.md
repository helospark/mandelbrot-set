# Mandelbrot and Julia fractal renderer.

Extendable fractal renderer, currently the following functions are implemented:

 - Rendering Mandelbrot set
 - Rendering Julia set
 - Rendering Julia sets at the edge of the Mandelbrot fractal (for a video)

Compile it on linux with (for maximum performance):
g++ *.cpp -pthread -std=c++11 -O3 -march=native

Run it with:
./a.out -f fractal_definitions/blue_mandelbrot.frak

Sample render of a Mandelbrot set:
![Sample render](https://raw.githubusercontent.com/helospark/mandelbrot-set/master/sample_renders/mandelbrot_1442164360.jpg)

Sample render of a Julia set:
![Sample render](https://raw.githubusercontent.com/helospark/mandelbrot-set/master/sample_renders/mandelbrot_1442164709.jpg)
