#include <d2d1.h>
#include <shlwapi.h>
#include <wincodec.h>
#include <wrl/client.h>

#include <stdexcept>

#include <kke/TextureRepository.hh>

#include "internal/HResult.hh"

#pragma comment(lib, "Shlwapi.lib")

using Microsoft::WRL::ComPtr;
using kke::internal::throwIfFailed;

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
	if (data == nullptr || size == 0) {
		throw std::invalid_argument("TextureRepository::load requires non-empty texture data.");
	}

	static ComPtr<IWICImagingFactory> factory;
	if (!factory) {
		throwIfFailed(
			CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(factory.ReleaseAndGetAddressOf())),
			"Failed to create WIC imaging factory");
	}

	ComPtr<IStream> stream;
	stream.Attach(SHCreateMemStream(reinterpret_cast<const uint8_t*>(data), static_cast<UINT>(size)));
	if (!stream) {
		throw std::runtime_error("Failed to create texture input stream.");
	}

	ComPtr<IWICBitmapDecoder> decoder;
	throwIfFailed(
		factory->CreateDecoderFromStream(stream.Get(), nullptr, WICDecodeMetadataCacheOnLoad, decoder.GetAddressOf()),
		"Failed to create WIC bitmap decoder");

	ComPtr<IWICBitmapFrameDecode> frame;
	throwIfFailed(
		decoder->GetFrame(0, frame.GetAddressOf()),
		"Failed to read WIC bitmap frame");

	ComPtr<IWICFormatConverter> converter;
	throwIfFailed(
		factory->CreateFormatConverter(converter.GetAddressOf()),
		"Failed to create WIC format converter");

	throwIfFailed(
		converter->Initialize(
			frame.Get(),
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.0f,
			WICBitmapPaletteTypeCustom),
		"Failed to initialize WIC format converter");

	ComPtr<ID2D1Bitmap> bitmap;
	throwIfFailed(
		deviceContext->CreateBitmapFromWicBitmap(converter.Get(), nullptr, bitmap.GetAddressOf()),
		"Failed to create D2D bitmap from texture data");

	const TextureId id = assignTexId();
	textures.emplace(id, bitmap);
	return id;
}

kke::TextureId kke::TextureRepository::assignTexId() {
	return nextId++;
}
