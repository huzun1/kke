#pragma once

#include <cstddef>
#include <memory>

#include "D2dTexture.hh"
#include "factory/TextureFactory.hh"
#include "kke/appearance/resource/texture/RawTextureData.hh"
#include "kke/engine/d2d/context/D2dContext.hh"
#include "kke/engine/d2d/d2d1_headers.hh"

namespace kke {
class TextureProvider {
	Microsoft::WRL::ComPtr<IWICImagingFactory2> imagingFactory;

  public:
	TextureProvider();

	std::shared_ptr<D2dTexture>
	uploadTexture(D2dContext const& context, void const* data, size_t size);

	std::shared_ptr<D2dTexture>
	uploadTexture(D2dContext const& context, RawTextureData const& data);

  private:
	void initialize();
};
} // namespace kke
