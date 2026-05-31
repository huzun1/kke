#pragma once

#include "kke/appearance/Scale.hh"
#include "kke/geometry/primitives/Point.hh"

namespace kke {
/**
 * @brief Scales subsequent drawing around a pivot point.
 */
struct Scaling {
	/**
	 * @brief Pivot point that remains fixed while scaling.
	 */
	Point center;

	/**
	 * @brief Horizontal and vertical scale multipliers.
	 */
	Scale scale;
};
} // namespace kke
