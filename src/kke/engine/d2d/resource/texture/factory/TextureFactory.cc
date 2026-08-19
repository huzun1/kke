#include "TextureFactory.hh"

#include "kke/utils/DebugLog.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

std::shared_ptr<D2dTexture> TextureFactory::create(
	D2dContext const& context, IWICImagingFactory2* imagingFactory, void const* data, size_t size
) {
	ComPtr<ID2D1Bitmap1> bitmap = createEncodedBitmap(context, imagingFactory, data, size);
	if (!bitmap) {
		return nullptr;
	}

	return std::make_shared<D2dTexture>(bitmap);
}

std::shared_ptr<D2dTexture>
TextureFactory::create(D2dContext const& context, RawTextureData const& data) {
	ComPtr<ID2D1Bitmap1> bitmap = createRawBitmap(context, data);
	if (!bitmap) {
		return nullptr;
	}

	return std::make_shared<D2dTexture>(bitmap);
}

ComPtr<ID2D1Bitmap1> TextureFactory::createEncodedBitmap(
	D2dContext const& context, IWICImagingFactory2* imagingFactory, void const* data, size_t size
) {
	if (!imagingFactory || !data || size == 0) {
		return nullptr;
	}

	ComPtr<IWICStream> stream;
	HRESULT result = imagingFactory->CreateStream(&stream);
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][TextureFactory] CreateStream failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
		return nullptr;
	}

	result = stream->InitializeFromMemory(
		reinterpret_cast<BYTE*>(const_cast<void*>(data)),
		static_cast<DWORD>(size)
	);
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][TextureFactory] InitializeFromMemory failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
		return nullptr;
	}

	ComPtr<IWICBitmapDecoder> decoder;
	result = imagingFactory->CreateDecoderFromStream(
		stream.Get(),
		nullptr,
		WICDecodeMetadataCacheOnLoad,
		&decoder
	);
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][TextureFactory] CreateDecoderFromStream failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
		return nullptr;
	}

	ComPtr<IWICBitmapFrameDecode> frame;
	result = decoder->GetFrame(0, &frame);
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][TextureFactory] GetFrame failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
		return nullptr;
	}

	ComPtr<IWICFormatConverter> converter;
	result = imagingFactory->CreateFormatConverter(&converter);
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][TextureFactory] CreateFormatConverter failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
		return nullptr;
	}

	result = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom
	);
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][TextureFactory] IWICFormatConverter::Initialize failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
		return nullptr;
	}

	ComPtr<ID2D1Bitmap1> bitmap;
	D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_NONE,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
		96.0f,
		96.0f
	);
	result = context.getDeviceContext()
				 ->CreateBitmapFromWicBitmap(converter.Get(), &properties, &bitmap);
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][TextureFactory] CreateBitmapFromWicBitmap failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
		return nullptr;
	}

	return bitmap;
}

ComPtr<ID2D1Bitmap1>
TextureFactory::createRawBitmap(D2dContext const& context, RawTextureData const& data) {
	if (!data.pixels || data.width == 0 || data.height == 0 ||
		data.stride < static_cast<size_t>(data.width) * 4) {
		return nullptr;
	}

	std::vector<uint8_t> convertedPixels = convertRgba8ToPbgra(data);
	if (convertedPixels.empty()) {
		return nullptr;
	}

	D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_NONE,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);
	D2D1_SIZE_U size = {data.width, data.height};

	ComPtr<ID2D1Bitmap1> bitmap;
	HRESULT result = context.getDeviceContext()->CreateBitmap(
		size,
		convertedPixels.data(),
		static_cast<UINT32>(data.width * 4),
		&properties,
		&bitmap
	);
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][TextureFactory] CreateBitmap failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
		return nullptr;
	}

	return bitmap;
}

std::vector<uint8_t> TextureFactory::convertRgba8ToPbgra(RawTextureData const& data) {
	std::vector<uint8_t> convertedPixels(static_cast<size_t>(data.width) * data.height * 4);
	uint8_t const* sourceBytes = static_cast<uint8_t const*>(data.pixels);

	for (uint32_t y = 0; y < data.height; ++y) {
		uint8_t const* sourceRow = sourceBytes + static_cast<size_t>(y) * data.stride;
		uint8_t* destinationRow = convertedPixels.data() + static_cast<size_t>(y) * data.width * 4;

		for (uint32_t x = 0; x < data.width; ++x) {
			uint8_t const* sourcePixel = sourceRow + static_cast<size_t>(x) * 4;
			uint8_t* destinationPixel = destinationRow + static_cast<size_t>(x) * 4;
			uint8_t alpha = sourcePixel[3];

			destinationPixel[0] = premultiply(sourcePixel[2], alpha);
			destinationPixel[1] = premultiply(sourcePixel[1], alpha);
			destinationPixel[2] = premultiply(sourcePixel[0], alpha);
			destinationPixel[3] = alpha;
		}
	}

	return convertedPixels;
}

uint8_t TextureFactory::premultiply(uint8_t color, uint8_t alpha) {
	return static_cast<uint8_t>((static_cast<uint32_t>(color) * alpha + 127u) / 255u);
}
