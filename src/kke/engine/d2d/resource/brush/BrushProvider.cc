#include "BrushProvider.hh"

#include "factory/BrushFactory.hh"
#include "hash/BrushHasher.hh"

using namespace kke;
using Microsoft::WRL::ComPtr;

BrushProvider::BrushProvider(uint32_t limit) : storage(limit) {
}

ComPtr<ID2D1Brush> BrushProvider::get(D2dContext const& context, Brush const& brush) {
	uint64_t key = BrushHasher::hash(brush);

	ComPtr<ID2D1Brush> cachedBrush = storage.get(key);
	if (cachedBrush) {
		return cachedBrush;
	}

	ComPtr<ID2D1Brush> createdBrush = BrushFactory::create(context, brush);
	if (!createdBrush) {
		return nullptr;
	}

	storage.put(key, createdBrush);
	return createdBrush;
}
