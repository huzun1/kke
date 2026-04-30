#pragma once

namespace kke {

/**
 * @brief Represents a 2D scale or size.
 *
 * This structure is used to describe dimensions or scaling factors
 * along the horizontal (x) and vertical (y) axes.
 *
 * It can represent:
 * - viewport sizes
 * - object dimensions
 * - scaling transformations
 */
struct Scale {
	/** @brief Horizontal component (width or X scale factor). */
	float x;

	/** @brief Vertical component (height or Y scale factor). */
	float y;

	/**
	 * @brief Constructs a Scale with the given components.
	 *
	 * @param x Horizontal component.
	 * @param y Vertical component.
	 */
	Scale(float x, float y);
};

#include "ScaleOperators.hh"

};	// namespace kke