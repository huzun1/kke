#pragma once

#include <cstddef>
#include <cstdint>

namespace kke {
/**
 * @brief Describes raw pixel data for texture upload.
 */
struct RawTextureData {
	/**
	 * @brief Pointer to the first pixel in memory.
	 */
	void const* pixels = nullptr;

	/**
	 * @brief Texture width in pixels.
	 */
	uint32_t width = 0;

	/**
	 * @brief Texture height in pixels.
	 */
	uint32_t height = 0;

	/**
	 * @brief Number of bytes between two adjacent rows.
	 */
	size_t stride = 0;
};
} // namespace kke
