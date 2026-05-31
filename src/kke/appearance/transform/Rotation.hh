#pragma once

#include "kke/geometry/primitives/Point.hh"

namespace kke {
/**
 * @brief Rotates subsequent drawing around a pivot point.
 */
struct Rotation {
	/**
	 * @brief Pivot point used as the center of rotation.
	 */
	Point center;

	/**
	 * @brief Rotation angle in degrees.
	 */
	float angle;
};
}	// namespace kke
