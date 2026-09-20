#pragma once

#include <cstddef>
#include <utility>

namespace kke::benchmark {
class ShadowBitmapDrawing {
	inline static bool isBitmapEnabled = false;
	inline static std::size_t drawCount = 0;

  public:
	static void setEnabled(bool value) {
		isBitmapEnabled = value;
	}

	static bool isEnabled() {
		return isBitmapEnabled;
	}

	static bool recordDraw(bool wasDrawn) {
		drawCount += wasDrawn;
		return wasDrawn;
	}

	static std::size_t consumeDrawCount() {
		return std::exchange(drawCount, 0);
	}
};
} // namespace kke::benchmark
