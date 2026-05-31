#pragma once

#include "kke/appearance/resource/texture/Texture.hh"
#include "kke/engine/d2d/d2d1_headers.hh"

namespace kke {
class D2dTexture : public Texture {
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap;

  public:
	D2dTexture(Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap);

	Microsoft::WRL::ComPtr<ID2D1Bitmap1> getBitmap() const;
};
} // namespace kke
