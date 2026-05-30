#include "TextureProvider.hh"

#include <cstdio>

using namespace kke;

TextureProvider::TextureProvider() {
	initialize();
}

std::shared_ptr<D2dTexture> TextureProvider::uploadTexture(
	D2dContext const& context,
	void const* data,
	size_t size) {
	return TextureFactory::create(context, imagingFactory.Get(), data, size);
}

std::shared_ptr<D2dTexture> TextureProvider::uploadTexture(
	D2dContext const& context,
	RawTextureData const& data) {
	return TextureFactory::create(context, data);
}

void TextureProvider::initialize() {
	HRESULT result = CoCreateInstance(
		CLSID_WICImagingFactory2,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(imagingFactory.ReleaseAndGetAddressOf()));
	if (FAILED(result)) {
		std::printf("[kke][TextureProvider] CoCreateInstance failed: 0x%08lx\n", static_cast<unsigned long>(result));
	}
}
