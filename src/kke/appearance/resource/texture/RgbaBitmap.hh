#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "kke/appearance/Color.hh"
#include "kke/appearance/resource/texture/RawTextureData.hh"

namespace kke {
class RgbaBitmap {
	std::vector<uint8_t> pixels;
	uint32_t bitmapWidth = 0;
	uint32_t bitmapHeight = 0;

  public:
	RgbaBitmap() = default;

	RgbaBitmap(uint32_t width, uint32_t height)
		: pixels(static_cast<size_t>(width) * height * 4), bitmapWidth(width),
		  bitmapHeight(height) {
	}

	uint32_t width() const {
		return bitmapWidth;
	}

	uint32_t height() const {
		return bitmapHeight;
	}

	size_t stride() const {
		return static_cast<size_t>(bitmapWidth) * 4;
	}

	bool empty() const {
		return pixels.empty();
	}

	uint8_t const* data() const {
		return pixels.data();
	}

	uint8_t* data() {
		return pixels.data();
	}

	void fill(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF) {
		for (uint32_t y = 0; y < bitmapHeight; ++y) {
			for (uint32_t x = 0; x < bitmapWidth; ++x) {
				setPixel(x, y, red, green, blue, alpha);
			}
		}
	}

	void fill(Color const& color) {
		fill(toByte(color.r), toByte(color.g), toByte(color.b), toByte(color.a));
	}

	void setPixel(
		uint32_t x, uint32_t y, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF
	) {
		if (x >= bitmapWidth || y >= bitmapHeight) {
			return;
		}

		size_t pixelIndex = (static_cast<size_t>(y) * bitmapWidth + x) * 4;
		pixels[pixelIndex + 0] = red;
		pixels[pixelIndex + 1] = green;
		pixels[pixelIndex + 2] = blue;
		pixels[pixelIndex + 3] = alpha;
	}

	void setPixel(uint32_t x, uint32_t y, Color const& color) {
		setPixel(x, y, toByte(color.r), toByte(color.g), toByte(color.b), toByte(color.a));
	}

	RawTextureData rawTextureData() const {
		return {
			.pixels = pixels.data(),
			.width = bitmapWidth,
			.height = bitmapHeight,
			.stride = stride(),
		};
	}

  private:
	static uint8_t toByte(float channel) {
		float clampedChannel = std::clamp(channel, 0.0f, 1.0f);
		return static_cast<uint8_t>(clampedChannel * 255.0f + 0.5f);
	}
};
} // namespace kke
