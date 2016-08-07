/*
 * ColorPaletteProvider.cpp
 *
 *  Created on: Jul 24, 2016
 *      Author: black
 */

#include "ColorPaletteProvider.h"

#include <vector>

#include "vector.h"

ColorPaletteProvider::ColorPaletteProvider() {

}

ColorPaletteProvider::~ColorPaletteProvider() {

}

std::shared_ptr<Vector> ColorPaletteProvider::getFromCache(const ColorPaletteRequest& request) {
	auto iterator = cache.find(request);
	if (iterator == cache.end()) {
		return std::shared_ptr<Vector>();
	} else {
		return (*iterator).second;
	}
}

std::shared_ptr<Vector> ColorPaletteProvider::generatePalette(const ColorPaletteRequest& request) {
	Vector* palette = new Vector[request.iterationLimit + 1];
	palette[request.iterationLimit] = request.maxIterationReachedColor;
	for (unsigned int i = 0; i < request.colors.size(); ++i) {
		int startIndex = (i == 0 ? 0 : ((int) ((request.iterationLimit * request.colors[i - 1].multiplier))));
		int endIndex = (int) ((request.iterationLimit * request.colors[i].multiplier));
		if (endIndex >= request.iterationLimit) {
			endIndex = request.iterationLimit - 1;
		}
		for (int j = startIndex; j < endIndex; ++j) {
			palette[j] = request.colors[i].startColor.interpolate(request.colors[i].endColor, (double) ((j - startIndex)) / (endIndex - startIndex));
		}
	}
	return std::shared_ptr<Vector>(palette, std::default_delete<Vector[]>());
}

void ColorPaletteProvider::addToCache(const ColorPaletteRequest& request, std::shared_ptr<Vector> palette) {
	cache[request] = palette;
}

std::shared_ptr<Vector> ColorPaletteProvider::createColorPalette(const ColorPaletteRequest& request) {
	std::shared_ptr<Vector> palette = generatePalette(request);
	addToCache(request, palette);
	return palette;
}

std::shared_ptr<Vector> ColorPaletteProvider::provideColorPalette(const ColorPaletteRequest& request) {
	std::shared_ptr<Vector> resultFromCache = getFromCache(request);
	if (resultFromCache != nullptr) {
		return resultFromCache;
	}
	return createColorPalette(request);
}

