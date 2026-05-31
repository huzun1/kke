#pragma once

/**
 * @brief Creates a Color from a 32-bit hexadecimal value (RGBA).
 *
 * The input format is expected to be 0xRRGGBBAA.
 * Each component is normalized to the range [0.0, 1.0].
 *
 * Example:
 * @code
 * auto color = KKE_COLOR_RGBA32(0xFF0000FF); // Red with full opacity
 * @endcode
 *
 * @param hex 32-bit hexadecimal color value.
 */
#define KKE_COLOR_RGBA32(hex) kke::Color( \
	((hex >> 24) & 0xFF) / 255.0f,      \
	((hex >> 16) & 0xFF) / 255.0f,      \
	((hex >> 8) & 0xFF) / 255.0f,       \
	(hex & 0xFF) / 255.0f)

/**
 * @brief Creates a Color from a 24-bit hexadecimal value (RGB).
 *
 * The input format is expected to be 0xRRGGBB.
 * Alpha will be set to 1.0 (fully opaque).
 *
 * Example:
 * @code
 * auto color = KKE_COLOR_RGB24(0x00FF00); // Green
 * @endcode
 *
 * @param hex 24-bit hexadecimal color value.
 */
#define KKE_COLOR_RGB24(hex) kke::Color( \
	((hex >> 16) & 0xFF) / 255.0f,      \
	((hex >> 8) & 0xFF) / 255.0f,       \
	(hex & 0xFF) / 255.0f)

namespace kke {

/**
 * @brief Represents an RGBA color.
 *
 * Each component is stored as a floating-point value in the range [0.0, 1.0].
 *
 * @note For convenient construction from hexadecimal color codes,
 *       use @ref KKE_COLOR_RGBA32 or @ref KKE_COLOR_RGB24.
 */
struct Color {
	/** @brief Red component. */
	float r;

	/** @brief Green component. */
	float g;

	/** @brief Blue component. */
	float b;

	/** @brief Alpha (opacity) component. */
	float a;

	/**
	 * @brief Constructs an opaque black color.
	 */
	Color();

	/**
	 * @brief Constructs a color from RGB values.
	 *
	 * Alpha is set to 1.0 (fully opaque).
	 *
	 * @param r Red component [0.0, 1.0].
	 * @param g Green component [0.0, 1.0].
	 * @param b Blue component [0.0, 1.0].
	 */
	Color(float r, float g, float b);

	/**
	 * @brief Constructs a color from RGBA values.
	 *
	 * @param r Red component [0.0, 1.0].
	 * @param g Green component [0.0, 1.0].
	 * @param b Blue component [0.0, 1.0].
	 * @param a Alpha component [0.0, 1.0].
	 */
	Color(float r, float g, float b, float a);
};
};	// namespace kke
