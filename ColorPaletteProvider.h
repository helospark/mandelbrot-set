/*
 * ColorPaletteProvider.h
 *
 *  Created on: Jul 24, 2016
 *      Author: black
 */

#ifndef COLORPALETTEPROVIDER_H_
#define COLORPALETTEPROVIDER_H_

#include <memory>
#include <vector>
#include <unordered_map>

#include "fractalParams.h"

class Vector;

class ColorPaletteRequest {
  public:
    std::vector<ColorInterpolation> colors;
    unsigned long iterationLimit;
    Vector maxIterationReachedColor;

    ColorPaletteRequest(const std::vector<ColorInterpolation>& colors, Vector maxIterationReachedColor, unsigned long iterationLimit) {
      this->colors = colors;
      this->maxIterationReachedColor = maxIterationReachedColor;
      this->iterationLimit = iterationLimit;
    }

    bool operator==(const ColorPaletteRequest& request) const {
      return this->colors == request.colors && this->iterationLimit == request.iterationLimit && this->maxIterationReachedColor == request.maxIterationReachedColor;
    }
};

namespace std {
  template<>
  struct hash<ColorPaletteRequest> {
      size_t operator()(const ColorPaletteRequest& request) const {
        size_t result = 0;
        result ^= hash<unsigned long>()(request.iterationLimit) ^ hash<Vector>()(request.maxIterationReachedColor);
        for (ColorInterpolation interpolation : request.colors) {
          result ^= hash<ColorInterpolation>()(interpolation);
        }
        return result;
      }
  };
}

class ColorPaletteProvider {
    std::unordered_map<ColorPaletteRequest, std::shared_ptr<Vector>> cache;

    std::shared_ptr<Vector> getFromCache(const ColorPaletteRequest& request);
    std::shared_ptr<Vector> generatePalette(const ColorPaletteRequest& request);
    void addToCache(const ColorPaletteRequest& request, std::shared_ptr<Vector> palette);
    std::shared_ptr<Vector> createColorPalette(const ColorPaletteRequest& request);
  public:
    ColorPaletteProvider();
    virtual ~ColorPaletteProvider();

    std::shared_ptr<Vector> provideColorPalette(const ColorPaletteRequest& request);
};

#endif /* COLORPALETTEPROVIDER_H_ */
