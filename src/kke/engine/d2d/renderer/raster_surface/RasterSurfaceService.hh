#pragma once

#include <memory>
#include <stack>

#include <d2d1_1.h>
#include <wrl/client.h>

#include "kke/appearance/Scale.hh"
#include "kke/appearance/resource/RasterSurface.hh"
#include "kke/engine/d2d/context/D2dEngineContext.hh"
#include "kke/geometry/shapes/Rect.hh"

namespace kke {
class D2dRasterSurface;

class RasterSurfaceService {
	struct RenderTargetState {
		Microsoft::WRL::ComPtr<ID2D1Image> target;
		D2D1_MATRIX_3X2_F transform;
		D2D1_TEXT_ANTIALIAS_MODE textAntialiasMode;
		float dpiX;
		float dpiY;
	};

	std::stack<RenderTargetState> renderTargetStates;

  public:
	std::shared_ptr<D2dRasterSurface>
	create(D2dEngineContext const& context, Scale const& logicalSize, float rasterScale);

	bool begin(D2dEngineContext const& context, std::shared_ptr<RasterSurface> const& surface);

	bool end(D2dEngineContext const& context);

	void draw(
		D2dEngineContext const& context,
		std::shared_ptr<RasterSurface> const& surface,
		Rect const& destination,
		float opacity
	);

  private:
	void pushCurrentRenderTarget(D2dEngineContext const& context);
};
} // namespace kke
