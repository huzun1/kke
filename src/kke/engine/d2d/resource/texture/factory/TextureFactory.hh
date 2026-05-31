#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "../D2dTexture.hh"
#include "kke/appearance/resource/texture/RawTextureData.hh"
#include "kke/engine/d2d/context/D2dContext.hh"
#include "kke/engine/d2d/d2d1_headers.hh"

namespace kke {
class TextureFactory {
  public:
	static std::shared_ptr<D2dTexture> create(
		D2dContext const& context,
		IWICImagingFactory2* imagingFactory,
		void const* data,
		size_t size
	);

	static std::shared_ptr<D2dTexture>
	create(D2dContext const& context, RawTextureData const& data);

  private:
	static Microsoft::WRL::ComPtr<ID2D1Bitmap1> createEncodedBitmap(
		D2dContext const& context,
		IWICImagingFactory2* imagingFactory,
		void const* data,
		size_t size
	);

	static Microsoft::WRL::ComPtr<ID2D1Bitmap1>
	createRawBitmap(D2dContext const& context, RawTextureData const& data);

	static std::vector<uint8_t> convertRgba8ToPbgra(RawTextureData const& data);

	static uint8_t premultiply(uint8_t color, uint8_t alpha);
};
} // namespace kke
