#include "TextureBrushFactory.hh"

#include <cmath>

#include "kke/engine/d2d/resource/texture/D2dTexture.hh"
#include "kke/engine/d2d/resource/texture/TextureSampling.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

ComPtr<ID2D1Brush>
TextureBrushFactory::create(D2dContext const& context, TextureBrush const& brush) {
	auto texture = std::dynamic_pointer_cast<D2dTexture>(brush.getTexture());
	if (!texture || !texture->getBitmap()) {
		return nullptr;
	}
	auto bitmap = texture->getBitmap();
	auto size = bitmap->GetSize();
	auto const& appearance = brush.getAppearance();
	Rect source = appearance.srcRect.value_or(Rect{{0.0f, 0.0f}, {size.width, size.height}});
	Rect const& destination = brush.getDestination();

	if (!isValidRect(source) || !isValidRect(destination) || !std::isfinite(appearance.opacity)) {
		return nullptr;
	}
	float scaleX = destination.width() / source.width();
	float scaleY = destination.height() / source.height();
	D2D1_IMAGE_BRUSH_PROPERTIES imageProperties{
		{source.min.x, source.min.y, source.max.x, source.max.y},
		D2D1_EXTEND_MODE_CLAMP,
		D2D1_EXTEND_MODE_CLAMP,
		TextureSampling::convert(appearance.interpolation),
	};
	D2D1_BRUSH_PROPERTIES properties = D2D1::BrushProperties(
		appearance.opacity,
		D2D1::Matrix3x2F::Scale(scaleX, scaleY) *
			D2D1::Matrix3x2F::Translation(destination.min.x, destination.min.y)
	);
	ComPtr<ID2D1ImageBrush> imageBrush;
	if (FAILED(context.getDeviceContext()
				   ->CreateImageBrush(bitmap.Get(), imageProperties, properties, &imageBrush))) {
		return nullptr;
	}
	ComPtr<ID2D1Brush> result;
	imageBrush.As(&result);
	return result;
}

bool TextureBrushFactory::isValidRect(Rect const& rect) {
	return std::isfinite(rect.min.x) && std::isfinite(rect.min.y) && std::isfinite(rect.max.x) &&
		   std::isfinite(rect.max.y) && rect.max.x > rect.min.x && rect.max.y > rect.min.y;
}
