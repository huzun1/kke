#pragma once

#include <d2d1.h>
#include <d2d1_1.h>

#include "brush/Brush.hpp"
#include "oreik/ResourceAllocator.hpp"

namespace oreik {
class Engine {
	ID2D1DeviceContext* deviceContext;

	ID2D1Bitmap1* renderTarget;
	ID2D1Bitmap* screenBitmap;
	oreik::ResourceAllocator resourceAllocator;

public:
	Engine(ID2D1DeviceContext* deviceContext);

	void begin(ID2D1Bitmap* screen);

	void end(ID2D1Image** output);

	void drawRect(D2D1_RECT_F rect, oreik::Brush const& brush, float strokeWidth);
};
};	// namespace oreik
