#pragma once

#include <memory>

#include "D2dCanvas.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"

namespace kke {
class CanvasFactory {
  public:
	static std::shared_ptr<D2dCanvas> createCanvas(const D2dEngineContext& context);
};
}; // namespace kke
