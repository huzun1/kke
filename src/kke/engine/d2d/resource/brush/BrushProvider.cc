#include "BrushProvider.hh"

#include <variant>

#include "factory/BrushFactory.hh"
#include "factory/TextureBrushFactory.hh"
#include "hash/BrushHasher.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

BrushProvider::BrushProvider(uint32_t limit) : storage(limit) {
}

ComPtr<ID2D1Brush> BrushProvider::get(D2dContext const& context, Brush const& brush) {
	if (std::holds_alternative<RasterSurfaceBrush>(brush)) {
		return BrushFactory::create(context, brush);
	}
	LinearGradientBrush const* gradientBrush = std::get_if<LinearGradientBrush>(&brush);
	TextureBrush const* textureBrush = std::get_if<TextureBrush>(&brush);
	uint64_t key;
	if (textureBrush != nullptr) {
		key = BrushHasher::hashTextureResource(*textureBrush);
	} else if (gradientBrush != nullptr) {
		key = BrushHasher::hashGradientStops(*gradientBrush);
	} else {
		key = BrushHasher::hash(brush);
	}

	ComPtr<ID2D1Brush> cachedBrush = storage.get(key);
	if (cachedBrush) {
		if (textureBrush != nullptr) {
			ComPtr<ID2D1ImageBrush> cachedImageBrush;
			if (SUCCEEDED(cachedBrush.As(&cachedImageBrush))) {
				return TextureBrushFactory::update(cachedImageBrush.Get(), *textureBrush)
						   ? cachedBrush
						   : nullptr;
			}
			return nullptr;
		}
		if (gradientBrush != nullptr) {
			ComPtr<ID2D1LinearGradientBrush> cachedGradientBrush;
			if (SUCCEEDED(cachedBrush.As(&cachedGradientBrush))) {
				BrushFactory::updateGradientLine(cachedGradientBrush.Get(), *gradientBrush);
			}
		}
		return cachedBrush;
	}

	ComPtr<ID2D1Brush> createdBrush = BrushFactory::create(context, brush);
	if (!createdBrush) {
		return nullptr;
	}

	storage.put(key, createdBrush);
	return createdBrush;
}
