#include <d2d1.h>
#include <shlwapi.h>
#include <wincodec.h>
#include <wrl/client.h>

#include <kke/TextureRepository.hpp>

#pragma comment(lib, "Shlwapi.lib")

kke::TextureRepository::TextureRepository(ID2D1DeviceContext* context)
	: deviceContext(context) {
}

ComPtr<ID2D1Bitmap> kke::TextureRepository::getTexture(TextureId id) {
	if (auto it = textures.find(id); it != textures.end()) {
		return it->second;
	}
	return nullptr;
}

bool kke::TextureRepository::exists(TextureId id) const {
	return textures.find(id) != textures.end();
}

void kke::TextureRepository::release(TextureId id) {
	textures.erase(id);
}

kke::TextureId kke::TextureRepository::load(const void* data, size_t size) {
	static IWICImagingFactory* factory = nullptr;
	if (!factory) {
		CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
	}

	auto stream = SHCreateMemStream(reinterpret_cast<const uint8_t*>(data), size);
	ComPtr<IWICBitmapDecoder> decoder;
	factory->CreateDecoderFromStream(stream, NULL, WICDecodeMetadataCacheOnLoad, &decoder);
	ComPtr<IWICBitmapFrameDecode> frame;
	decoder->GetFrame(0, &frame);
	ComPtr<IWICFormatConverter> converter;
	factory->CreateFormatConverter(&converter);

	converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.0f,
		WICBitmapPaletteTypeCustom);

	ComPtr<ID2D1Bitmap> bitmap;
	deviceContext->CreateBitmapFromWicBitmap(converter.Get(), NULL, &bitmap);

	TextureId id = assignTexId();
	textures.emplace(id, bitmap);

	return id;
}

kke::TextureId kke::TextureRepository::assignTexId() {
	return nextId++;
}
