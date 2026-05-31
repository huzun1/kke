#pragma once

namespace kke {
/**
 * @brief Backend-owned uploaded font resource.
 */
class Font {
  public:
	/**
	 * @brief Destroys the backend font resource.
	 */
	virtual ~Font() = default;
};
} // namespace kke
