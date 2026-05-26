#pragma once

#include <cstdint>

#include "kke/appearance/resource/brush/Brush.hh"
#include "kke/engine/d2d/context/D2dContext.hh"
#include "kke/engine/d2d/d2d1_headers.hh"
#include "kke/engine/d2d/resource/KeyCacheStorage.hh"

namespace kke {
class BrushProvider {
	KeyCacheStorage<ID2D1Brush> storage;

public:
	BrushProvider(uint32_t limit = UINT32_MAX);

	Microsoft::WRL::ComPtr<ID2D1Brush> get(D2dContext const& context, Brush const& brush);
};
}	// namespace kke
