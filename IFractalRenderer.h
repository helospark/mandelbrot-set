/*
 * IFractalRenderer.h
 *
 *  Created on: Jul 24, 2016
 *      Author: black
 */

#ifndef IFRACTALRENDERER_H_
#define IFRACTALRENDERER_H_
#include <string>
#include <map>
#include "fractalParams.h"

class IFractalRenderer {
public:
	virtual ~IFractalRenderer() {}
	virtual bool isSupported(const std::string& type, std::map<std::string, std::string>& params) = 0;
	virtual bool supportsComputeDevice(const std::string& type) = 0;
	virtual std::string description() = 0;
	virtual std::string name() = 0;
	virtual void render(const FractalParams& params, const std::map<std::string, std::string>& arguments) = 0;
};



#endif /* IFRACTALRENDERER_H_ */
