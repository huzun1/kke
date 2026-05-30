#pragma once

#include <cstddef>
#include <cstdint>

namespace kke {
struct RawTextureData {
	void const* pixels = nullptr;
	uint32_t width = 0;
	uint32_t height = 0;
	size_t stride = 0;
};
}	// namespace kke
