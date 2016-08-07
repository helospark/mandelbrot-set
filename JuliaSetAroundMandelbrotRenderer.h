/*
 * SimpleMandelbrotJuliaRenderer.h
 *
 *  Created on: Jul 24, 2016
 *      Author: black
 */

#ifndef JULIA_SET_AROUND_MANDELBROT_RENDERER_H_
#define JULIA_SET_AROUND_MANDELBROT_RENDERER_H_
#include <memory>
#include "CommandLineArguments.h"
#include "IFractalRenderer.h"
#include "fractalDrawer.h"
#include "point2d.h"
#include "bmp.h"
#include "FractalType.h"
#include "ThreadedMandelbrotJuliaImageGenerator.h"
#include "ThreadUtils.h"

class JuliaSetAroundMandelbrotRenderer: public IFractalRenderer {
    IFractalRenderer* fractalRenderer;
    ThreadedMandelbrotJuliaImageGenerator* mandelbrotJuliaImageGenerator;
    FractalDrawer* fractalDrawer;

    void drawJuliaSet(const Complex& complex, FractalParams params, int imageIndex, int intepolationIndex, const std::map<std::string, std::string>& arguments);
    std::vector<Point2d> findOutlinePoints(std::shared_ptr<Bmp> mandelbrot);
    bool isOutlineFinished(int count, Point2d point, Point2d startPoint);
    bool findNextPoint(std::shared_ptr<Bmp> mandelbrot, Point2d startPoint, Point2d* current);
    bool isNextToMandelbrotSet(std::shared_ptr<Bmp> mandelbrot, Point2d current);
    Point2d findStartPoint(std::shared_ptr<Bmp> mandelbrot);
    std::shared_ptr<Bmp> drawMandelbrot(FractalParams fractalParams, const std::map<std::string, std::string>& arguments);
  public:
    JuliaSetAroundMandelbrotRenderer(IFractalRenderer* fractalRenderer, ThreadedMandelbrotJuliaImageGenerator* mandelbrotJuliaImageGenerator, FractalDrawer* fractalDrawer) {
      this->fractalRenderer = fractalRenderer;
      this->fractalDrawer = fractalDrawer;
      this->mandelbrotJuliaImageGenerator = mandelbrotJuliaImageGenerator;
    }

    virtual ~JuliaSetAroundMandelbrotRenderer() {

    }

    virtual bool isSupported(const std::string& type, std::map<std::string, std::string>& params) override {
      if (type != FractalType::JULIA_AROUND_MANDELBROT) {
        return false;
      }
      auto iterator = params.find(CommandLineArgumentConstants::COMPUTE_DEVICE_TYPE);
      return (iterator == params.end() || supportsComputeDevice((*iterator).second));
    }

    virtual bool supportsComputeDevice(const std::string& type) override {
      return fractalRenderer->supportsComputeDevice(type);
    }

    virtual std::string description() override {
      return "Renders the Julia sets around the Mandelbrot set (aka. outline of it)";
    }

    virtual std::string name() override {
      return "JuliaSetAroundMandelbrotRenderer";
    }

    virtual void render(const FractalParams& params, const std::map<std::string, std::string>& arguments) override;
};

#endif /* SIMPLECPUMANDELBROTJULIARENDERER_H_ */
