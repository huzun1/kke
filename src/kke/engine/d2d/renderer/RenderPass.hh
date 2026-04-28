#pragma once

#include "kke/engine/d2d/D2dContext.hh"
#include "kke/engine/d2d/d2d1_headers.hh"

namespace kke {
class RenderPass {
	ID2D1Bitmap* lastRenderTarget = nullptr;

public:
	void beginDraw(D2dContext& context, ID2D1Bitmap* renderTarget);

	void endDraw(D2dContext& context);

	void clear(D2dContext& context);

private:
	static Microsoft::WRL::ComPtr<ID2D1Bitmap> createBitmapCopy(ID2D1DeviceContext* deviceContext, ID2D1Bitmap* source);
};
};	// namespace kke
