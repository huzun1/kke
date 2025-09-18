#include <d2d1.h>
#include <shlwapi.h>
#include <wincodec.h>
#include <wrl/client.h>

#include <oreik/TextureRepository.hpp>

#pragma comment(lib, "Shlwapi.lib")

oreik::TextureRepository::TextureRepository(ID2D1DeviceContext* context)
	: deviceContext(context) {
}

Microsoft::WRL::ComPtr<ID2D1Bitmap> oreik::TextureRepository::fetchTexture(size_t index) {
	if (textures.empty() || index >= textures.size()) {
		return nullptr;
	}
	return textures[index];
}

size_t oreik::TextureRepository::load(const void* data, size_t size) {
	static IWICImagingFactory* factory = nullptr;
	if (!factory) {
		CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
	}

	auto stream = SHCreateMemStream(reinterpret_cast<const unsigned char*>(data), size);

	IWICBitmapDecoder* decoder;
	factory->CreateDecoderFromStream(stream, NULL, WICDecodeMetadataCacheOnLoad, &decoder);

	IWICBitmapFrameDecode* frame;
	decoder->GetFrame(0, &frame);

	IWICFormatConverter* converter;
	factory->CreateFormatConverter(&converter);
	converter->Initialize(
		frame,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.0f,
		WICBitmapPaletteTypeCustom);
	stream->Release();
	frame->Release();
	decoder->Release();

	Microsoft::WRL::ComPtr<ID2D1Bitmap> bitmap;
	deviceContext->CreateBitmapFromWicBitmap(converter, NULL, &bitmap);
	textures.push_back(bitmap);

	return textures.size() - 1;	 // Return index of loaded bitmap
}
