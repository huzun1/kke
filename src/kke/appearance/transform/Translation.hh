#pragma once

#include "kke/geometry/primitives/Point.hh"

namespace kke {
/**
 * @brief Translates subsequent drawing by a fixed offset.
 */
struct Translation {
	/**
	 * @brief Horizontal and vertical offset applied to the source.
	 */
	Point offset;
};
}	// namespace kke
