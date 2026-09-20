#include "TextureBrushFactory.hh"

#include <cmath>

#include "kke/engine/d2d/resource/texture/D2dTexture.hh"
#include "kke/engine/d2d/resource/texture/TextureSampling.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

ComPtr<ID2D1Brush>
TextureBrushFactory::create(D2dContext const& context, TextureBrush const& brush) {
	ComPtr<ID2D1ImageBrush> imageBrush;
	D2D1_IMAGE_BRUSH_PROPERTIES imageProperties{};
	HRESULT result =
		context.getDeviceContext()->CreateImageBrush(nullptr, imageProperties, &imageBrush);
	if (FAILED(result) || !update(imageBrush.Get(), brush)) {
		return nullptr;
	}
	return imageBrush;
}

bool TextureBrushFactory::update(ID2D1ImageBrush* imageBrush, TextureBrush const& brush) {
	auto texture = std::dynamic_pointer_cast<D2dTexture>(brush.getTexture());
	if (!imageBrush || !texture || !texture->getBitmap()) {
		return false;
	}
	auto bitmap = texture->getBitmap();
	auto size = bitmap->GetSize();
	auto const& appearance = brush.getAppearance();
	Rect source = appearance.srcRect.value_or(Rect{{0.0f, 0.0f}, {size.width, size.height}});
	Rect const& destination = brush.getDestination();

	if (!isValidRect(source) || !isValidRect(destination) || !std::isfinite(appearance.opacity)) {
		return false;
	}
	float scaleX = destination.width() / source.width();
	float scaleY = destination.height() / source.height();
	D2D1_RECT_F sourceRectangle{source.min.x, source.min.y, source.max.x, source.max.y};
	imageBrush->SetImage(bitmap.Get());
	imageBrush->SetSourceRectangle(&sourceRectangle);
	imageBrush->SetExtendModeX(D2D1_EXTEND_MODE_CLAMP);
	imageBrush->SetExtendModeY(D2D1_EXTEND_MODE_CLAMP);
	imageBrush->SetInterpolationMode(TextureSampling::convert(appearance.interpolation));
	imageBrush->SetOpacity(appearance.opacity);
	imageBrush->SetTransform(
		D2D1::Matrix3x2F::Scale(scaleX, scaleY) *
		D2D1::Matrix3x2F::Translation(destination.min.x, destination.min.y)
	);
	return true;
}

bool TextureBrushFactory::isValidRect(Rect const& rect) {
	return std::isfinite(rect.min.x) && std::isfinite(rect.min.y) && std::isfinite(rect.max.x) &&
		   std::isfinite(rect.max.y) && rect.max.x > rect.min.x && rect.max.y > rect.min.y;
}
