#pragma once

#include "kke/engine/d2d/D2dContext.hh"
#include "kke/engine/d2d/d2d1_headers.hh"

namespace kke {
class RenderPass {
public:
	void beginDraw(D2dContext const& context, ID2D1Image* renderTarget);

	void endDraw();

	void clear();
};
};	// namespace kke
