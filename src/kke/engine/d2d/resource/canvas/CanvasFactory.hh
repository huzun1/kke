#pragma once

#include <memory>

#include "D2dCanvas.hh"
#include "kke/engine/d2d/D2dContext.hh"

namespace kke {
class CanvasFactory {
public:
	static std::shared_ptr<D2dCanvas> createCanvas(const D2dContext& context);
};
};	// namespace kke
