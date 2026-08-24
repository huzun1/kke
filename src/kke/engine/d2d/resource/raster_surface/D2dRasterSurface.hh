#pragma once

#include <d2d1_1.h>
#include <wrl/client.h>

#include "kke/appearance/resource/RasterSurface.hh"

namespace kke {
class D2dRasterSurface : public RasterSurface {
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap;

  public:
	explicit D2dRasterSurface(Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap);

	Microsoft::WRL::ComPtr<ID2D1Bitmap1> const& getBitmap() const;
};
} // namespace kke
