#pragma once

#include <d2d1.h>
#include <d2d1_1.h>

namespace oreik {
class Engine {
	ID2D1DeviceContext* deviceContext;

	ID2D1Bitmap1* rendererTarget;
	ID2D1Bitmap* screenBitmap;

public:
	Engine(ID2D1DeviceContext* deviceContext);

	void begin(ID2D1Bitmap* screen);

	void end(ID2D1Image** output);
};
};	// namespace oreik
