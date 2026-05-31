#pragma once

namespace kke {
/**
 * @brief Backend-owned offscreen render target.
 *
 * Instances are created by the engine and can later be drawn or used as effect sources.
 */
class Canvas {
  public:
	/**
	 * @brief Destroys the backend canvas resource.
	 */
	virtual ~Canvas() = default;
};
} // namespace kke
