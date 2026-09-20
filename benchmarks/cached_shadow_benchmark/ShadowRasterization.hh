#pragma once

#include <cstddef>

#include "kke/engine/d2d/context/D2dEngineContext.hh"

namespace kke::benchmark {
class ShadowRasterization {
	inline static float rasterScale = 1.0f;
	inline static std::size_t generatedBytes = 0;
	inline static std::size_t generatedCount = 0;

  public:
	static void configure(float scale);

	static float scale();

	static Microsoft::WRL::ComPtr<ID2D1Bitmap1> create(
		D2dEngineContext& context,
		Microsoft::WRL::ComPtr<ID2D1Image> const& image,
		D2D1_RECT_F const& bounds
	);

	static void recordBitmap(ID2D1Bitmap1* bitmap);

	static std::size_t bitmapBytes();

	static std::size_t bitmapCount();
};
} // namespace kke::benchmark
