/*
 * SimpleMandelbrotJuliaRenderer.h
 *
 *  Created on: Jul 24, 2016
 *      Author: black
 */

#ifndef SIMPLECPUMANDELBROTJULIARENDERER_H_
#define SIMPLECPUMANDELBROTJULIARENDERER_H_
#include "CommandLineArguments.h"
#include "IFractalRenderer.h"
#include "FractalType.h"
#include "ThreadedMandelbrotJuliaImageGenerator.h"
#include "ThreadUtils.h"

class SimpleCpuMandelbrotJuliaRenderer : public IFractalRenderer {
	ThreadedMandelbrotJuliaImageGenerator* imageGenerator;
	FractalDrawer* fractalDrawer;
public:
	SimpleCpuMandelbrotJuliaRenderer(ThreadedMandelbrotJuliaImageGenerator* fractalRenderer, FractalDrawer* fractalDrawer) {
		this->imageGenerator = fractalRenderer;
		this->fractalDrawer = fractalDrawer;
	}

	virtual ~SimpleCpuMandelbrotJuliaRenderer() {

	}

	virtual bool isSupported(const std::string& type, std::map<std::string, std::string>& params) override {
		if (type != FractalType::MANDELBROT && type != FractalType::JULIA) {
			return false;
		}
		auto iterator = params.find(CommandLineArgumentConstants::COMPUTE_DEVICE_TYPE);
		return (iterator == params.end() || supportsComputeDevice((*iterator).second));
	}

	virtual bool supportsComputeDevice(const std::string& type) override {
		return type == CommandLineArgumentConstants::COMPUTE_DEVICE_CPU;
	}

	virtual std::string description() override {
		return "Renders Mandelbrot set factals";
	}

	virtual std::string name() override {
		return "CPU - SimpleMandelbrotJuliaRenderer";
	}

	virtual void render(const FractalParams& params, const std::map<std::string, std::string>& arguments) override {
		std::shared_ptr<Bmp> result = imageGenerator->generateImage(params, ThreadUtils::getThreadCountFromArgument(arguments));
		fractalDrawer->draw(result, params.fileName);
	}
};

#endif /* SIMPLECPUMANDELBROTJULIARENDERER_H_ */
