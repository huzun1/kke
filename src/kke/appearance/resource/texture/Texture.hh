#pragma once

namespace kke {
/**
 * @brief Backend-owned uploaded texture resource.
 */
class Texture {
  public:
	/**
	 * @brief Destroys the backend texture resource.
	 */
	virtual ~Texture() = default;
};
} // namespace kke
