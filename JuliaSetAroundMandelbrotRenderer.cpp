/*
 * JuliaSetAroundMandelbrotRenderer.cpp
 *
 *  Created on: Jul 24, 2016
 *      Author: black
 */
#include "JuliaSetAroundMandelbrotRenderer.h"
#include "Rectangle.h"
#include "math.h"

Vector visitedNodeColor(255, 0, 0);
Vector WHITE_PIXEL(255, 255, 255);
Vector BLACK_PIXEL(0, 0, 0);
const int interpolationCount = 3;
const double MIN_X = -2.2;
const double MAX_X = 1.4;
const double MIN_Y = -1.2;
const double MAX_Y = 1.2;
const int WIDTH = 1920 * 3;
const int HEIGHT = 1080 * 3;
const int ITERATION_LIMIT = 500;
const int START_INTEX = 27877-1221;

std::shared_ptr<Bmp> JuliaSetAroundMandelbrotRenderer::drawMandelbrot(FractalParams params) {
  params.colors.clear();
  params.colors.push_back(ColorInterpolation(WHITE_PIXEL, WHITE_PIXEL, 0.5));
  params.colors.push_back(ColorInterpolation(BLACK_PIXEL, BLACK_PIXEL, 0.5));
  params.colors.push_back(ColorInterpolation(BLACK_PIXEL, BLACK_PIXEL, 1.0));
  params.minX = MIN_X;
  params.maxX = MAX_X;
  params.minY = MIN_Y;
  params.maxY = MAX_Y;
  params.width = WIDTH;
  params.height = HEIGHT;
  params.iterationLimit = ITERATION_LIMIT;
  params.type = "mandelbrot";
  params.maxIterationReachedColor = Vector(BLACK_PIXEL);
  std::shared_ptr<Bmp> bmp = mandelbrotJuliaImageGenerator->generateImage(params);
  return bmp;
}

Point2d JuliaSetAroundMandelbrotRenderer::searchStartPoint(std::shared_ptr<Bmp> mandelbrot) {
  Point2d result = Point2d(0, mandelbrot->getHeight() / 2);
  while (result.x < mandelbrot->getWidth() && mandelbrot->getPixel(result).getX() > 250) {
    result.x += 1;
  }
  result.x -= 1;
  return result;
}

bool JuliaSetAroundMandelbrotRenderer::isNextToInterestingPoint(std::shared_ptr<Bmp> mandelbrot, Point2d current) {
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      Point2d newPoint = Point2d(current.x + j, current.y + i);
      if (i == j || -i == j) {
        continue;
      }
      if (mandelbrot->getPixel(newPoint).getX() < 250) {
        return true;
      }
    }
  }
  return false;
}

bool JuliaSetAroundMandelbrotRenderer::findNextPoint(std::shared_ptr<Bmp> mandelbrot, Point2d startPoint, Point2d* current) {
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      Point2d newPoint(current->x + j, current->y + i);
      if (i == 0 && j == 0) {
        continue;
      } else if (mandelbrot->getPixel(newPoint) == visitedNodeColor || mandelbrot->getPixel(newPoint).getX() < 1.0) {
        continue;
      } else if (!isNextToInterestingPoint(mandelbrot, newPoint)) {
        continue;
      }
      current->x = newPoint.x;
      current->y = newPoint.y;
      return true;
    }
  }
  return false;
}

bool JuliaSetAroundMandelbrotRenderer::isOutlineFinished(int count, Point2d point, Point2d startPoint) {
  if (count > 10) {
    for (int i = -1; i <= 1; ++i) {
      for (int j = -1; j <= 1; ++j) {
        if (Point2d(point.x + j, point.y + i) == startPoint) {
          return true;
        }
      }
    }
  }
  return false;
}

std::vector<Point2d> JuliaSetAroundMandelbrotRenderer::findOutlineList(std::shared_ptr<Bmp> mandelbrot) {
  Point2d startPoint = searchStartPoint(mandelbrot);
  std::vector<Point2d> result;
  result.push_back(startPoint);
  mandelbrot->setPixel(startPoint, visitedNodeColor);
  Point2d currentPoint = startPoint;
  std::cout << "Starting loop" << std::endl;
  while (!isOutlineFinished(result.size(), currentPoint, startPoint)) {
    currentPoint = result.back();
    mandelbrot->setPixel(currentPoint, visitedNodeColor);
    bool found = findNextPoint(mandelbrot, startPoint, &currentPoint);
    if (found) {
      result.push_back(currentPoint);
    } else {
      if (result.size() <= 10) {
        fractalDrawer->draw(mandelbrot, "debug");
        break;
      }
      result.pop_back();
    }
  }
  return result;
}

void JuliaSetAroundMandelbrotRenderer::drawJuliaSet(const Complex& complex, FractalParams params, int index, const std::map<std::string, std::string>& arguments) {
  params.type = "julia";
  params.juliaSetStartParameter = complex;
  fractalRenderer->render(params, arguments);
}

void JuliaSetAroundMandelbrotRenderer::render(const FractalParams& params, const std::map<std::string, std::string>& arguments) {
  std::shared_ptr<Bmp> mandelbrot = drawMandelbrot(params);
  std::vector<Point2d> outline = findOutlineList(mandelbrot);
  std::cout << "Drawing " << outline.size() << " * " << interpolationCount << " images" << std::endl;
  Rectangle origin(0, WIDTH, 0, HEIGHT);
  Rectangle destination(MIN_X, MAX_X, MIN_Y, MAX_Y);
  Complex complex = Math::mapComplex(outline[START_INTEX], origin, destination);
  for (int i = START_INTEX; i < outline.size(); ++i) {
    Complex newComplex = Math::mapComplex(outline[i], origin, destination);
    std::cout << newComplex << ' ' << i << '\n';
    for (int j = 0; j < interpolationCount; ++j) {
      Complex interpolated = Math::interpolate(complex, newComplex, (double) j / interpolationCount);
      drawJuliaSet(interpolated, params, j, arguments);
    }
    complex = newComplex;
  }
}
