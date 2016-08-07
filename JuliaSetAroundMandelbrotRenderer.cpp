/*
 * JuliaSetAroundMandelbrotRenderer.cpp
 *
 *  Created on: Jul 24, 2016
 *      Author: black
 */
#include "JuliaSetAroundMandelbrotRenderer.h"
#include "Rectangle.h"
#include "math.h"
#include <sstream>

const Vector VISITED_NODE_COLOR(255, 0, 0);
const Vector BACKGOUND_COLOR(255, 255, 255);
const Vector MANDELBROT_COLOR(0, 0, 0);

const double MANDELBROT_MIN_X = -2.2;
const double MANDELBROT_MAX_X = 1.4;
const double MANDELBROT_MIN_Y = -1.2;
const double MANDELBROT_MAX_Y = 1.2;
const double MANDELBROT_BLACK_THRESHOLD = 0.5;
const int MANDELBROT_ITERATION_LIMIT = 500;
const int START_INTEX = 400;
const double INTEGER_COMPARE_EPSILON = 0.1;
const int MINIMUM_NUMBER_OF_POINTS_FOR_OUTLINE = 10;

std::shared_ptr<Bmp> JuliaSetAroundMandelbrotRenderer::drawMandelbrot(FractalParams fractalParams, const std::map<std::string, std::string>& arguments) {
  fractalParams.colors.clear();
  fractalParams.colors.push_back(ColorInterpolation(BACKGOUND_COLOR, BACKGOUND_COLOR, MANDELBROT_BLACK_THRESHOLD));
  fractalParams.colors.push_back(ColorInterpolation(MANDELBROT_COLOR, MANDELBROT_COLOR, MANDELBROT_BLACK_THRESHOLD));
  fractalParams.colors.push_back(ColorInterpolation(MANDELBROT_COLOR, MANDELBROT_COLOR, 1.0));
  fractalParams.minX = MANDELBROT_MIN_X;
  fractalParams.maxX = MANDELBROT_MAX_X;
  fractalParams.minY = MANDELBROT_MIN_Y;
  fractalParams.maxY = MANDELBROT_MAX_Y;
  fractalParams.width = fractalParams.outlineMandelbrotWidth;
  fractalParams.height = fractalParams.outlineMandelbrotHeight;
  fractalParams.iterationLimit = MANDELBROT_ITERATION_LIMIT;
  fractalParams.type = FractalType::MANDELBROT;
  fractalParams.maxIterationReachedColor = Vector(MANDELBROT_COLOR);
  std::shared_ptr<Bmp> bmp = mandelbrotJuliaImageGenerator->generateImage(fractalParams, ThreadUtils::getThreadCountFromArgument(arguments));
  return bmp;
}

Point2d JuliaSetAroundMandelbrotRenderer::findStartPoint(std::shared_ptr<Bmp> mandelbrot) {
  Point2d result = Point2d(0, mandelbrot->getHeight() / 2);
  while (result.x < mandelbrot->getWidth() && mandelbrot->getPixel(result).equalsWithThreshold(BACKGOUND_COLOR, INTEGER_COMPARE_EPSILON)) {
    result.x += 1;
  }
  result.x -= 1;
  return result;
}

bool isDiagonal(int i, int j) {
   return i == j || -i == j;
}

bool JuliaSetAroundMandelbrotRenderer::isNextToMandelbrotSet(std::shared_ptr<Bmp> mandelbrot, Point2d current) {
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      Point2d newPoint = Point2d(current.x + j, current.y + i);
      if (isDiagonal(i, j)) {
        continue;
      }
      if (mandelbrot->getPixel(newPoint).equalsWithThreshold(MANDELBROT_COLOR, INTEGER_COMPARE_EPSILON)) {
        return true;
      }
    }
  }
  return false;
}

bool JuliaSetAroundMandelbrotRenderer::findNextPoint(std::shared_ptr<Bmp> mandelbrot, Point2d startPoint, Point2d* result) {
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      Point2d newPoint(result->x + j, result->y + i);
      if (i == 0 && j == 0) {
        continue;
      } else if (mandelbrot->getPixel(newPoint).equalsWithThreshold(VISITED_NODE_COLOR, INTEGER_COMPARE_EPSILON)
          || mandelbrot->getPixel(newPoint).equalsWithThreshold(MANDELBROT_COLOR, INTEGER_COMPARE_EPSILON)) {
        continue;
      } else if (!isNextToMandelbrotSet(mandelbrot, newPoint)) {
        continue;
      }
      result->x = newPoint.x;
      result->y = newPoint.y;
      return true;
    }
  }
  return false;
}

bool JuliaSetAroundMandelbrotRenderer::isOutlineFinished(int count, Point2d point, Point2d startPoint) {
  if (count > MINIMUM_NUMBER_OF_POINTS_FOR_OUTLINE) {
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

std::vector<Point2d> JuliaSetAroundMandelbrotRenderer::findOutlinePoints(std::shared_ptr<Bmp> mandelbrot) {
  Point2d startPoint = findStartPoint(mandelbrot);
  std::vector<Point2d> result;
  result.push_back(startPoint);
  Point2d currentPoint = startPoint;
  while (!isOutlineFinished(result.size(), currentPoint, startPoint)) {
    currentPoint = result.back();
    mandelbrot->setPixel(currentPoint, VISITED_NODE_COLOR);
    bool found = findNextPoint(mandelbrot, startPoint, &currentPoint);
    if (found) {
      result.push_back(currentPoint);
    } else {
      if (result.size() == 0) {
        fractalDrawer->draw(mandelbrot, "debugRender");
        break;
      }
      result.pop_back();
    }
  }
  return result;
}

void JuliaSetAroundMandelbrotRenderer::drawJuliaSet(const Complex& complex, FractalParams params, int imageIndex, int interpolationIndex, const std::map<std::string, std::string>& arguments) {
  params.type = FractalType::JULIA;
  params.juliaSetStartParameter = complex;
  std::stringstream ss;
  ss << params.fileName << "_" << imageIndex << "_" << interpolationIndex;
  params.fileName = ss.str();
  fractalRenderer->render(params, arguments);
}

void JuliaSetAroundMandelbrotRenderer::render(const FractalParams& params, const std::map<std::string, std::string>& arguments) {
  std::shared_ptr<Bmp> mandelbrot = drawMandelbrot(params, arguments);
  std::vector<Point2d> outline = findOutlinePoints(mandelbrot);

  std::cout << "Rendering " << outline.size() << " × " << params.outlineInterpolationCount << " images" << std::endl;

  Rectangle mandelbrotPixelSpaceDimensions(0, params.outlineMandelbrotWidth, 0, params.outlineMandelbrotHeight);
  Rectangle mandelbrotComplexPlaneDimensions(MANDELBROT_MIN_X, MANDELBROT_MAX_X, MANDELBROT_MIN_Y, MANDELBROT_MAX_Y);

  Complex previousMandelbrotPoint = Math::mapComplex(outline[START_INTEX], mandelbrotPixelSpaceDimensions, mandelbrotComplexPlaneDimensions);

  for (int i = START_INTEX; i < outline.size(); ++i) {
    Complex currentMadelbrotPoint = Math::mapComplex(outline[i], mandelbrotPixelSpaceDimensions, mandelbrotComplexPlaneDimensions);
    std::cout << i << "\t-\t" << currentMadelbrotPoint << '\n';
    for (int j = 0; j < params.outlineInterpolationCount; ++j) {
      Complex interpolated = Math::interpolate(previousMandelbrotPoint, currentMadelbrotPoint, (double) j / params.outlineInterpolationCount);
      drawJuliaSet(interpolated, params, i, j, arguments);
    }
    previousMandelbrotPoint = currentMadelbrotPoint;
  }
}
