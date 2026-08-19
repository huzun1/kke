#include "BrushProvider.hh"

#include <variant>

#include "factory/BrushFactory.hh"
#include "hash/BrushHasher.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

BrushProvider::BrushProvider(uint32_t limit) : storage(limit) {
}

ComPtr<ID2D1Brush> BrushProvider::get(D2dContext const& context, Brush const& brush) {
	LinearGradientBrush const* gradientBrush = std::get_if<LinearGradientBrush>(&brush);
	uint64_t key = gradientBrush == nullptr ? BrushHasher::hash(brush)
											: BrushHasher::hashGradientStops(*gradientBrush);

	ComPtr<ID2D1Brush> cachedBrush = storage.get(key);
	if (cachedBrush) {
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
