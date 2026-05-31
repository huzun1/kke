#pragma once

namespace kke {
struct Scale;

/**
 * @brief Returns a copy of `lhs` with `rhs` added to both components.
 */
Scale operator+(Scale lhs, float rhs);

/**
 * @brief Returns the component-wise sum of two scales.
 */
Scale operator+(Scale lhs, Scale const& rhs);

/**
 * @brief Returns a copy of `lhs` with `rhs` subtracted from both components.
 */
Scale operator-(Scale lhs, float rhs);

/**
 * @brief Returns the component-wise difference of two scales.
 */
Scale operator-(Scale lhs, Scale const& rhs);

/**
 * @brief Returns a copy of `lhs` with both components multiplied by `rhs`.
 */
Scale operator*(Scale lhs, float rhs);

/**
 * @brief Returns the component-wise product of two scales.
 */
Scale operator*(Scale lhs, Scale const& rhs);

/**
 * @brief Returns a copy of `lhs` with both components divided by `rhs`.
 */
Scale operator/(Scale lhs, float rhs);

/**
 * @brief Returns the component-wise quotient of two scales.
 */
Scale operator/(Scale lhs, Scale const& rhs);

/**
 * @brief Adds `rhs` to both components of `lhs`.
 */
Scale& operator+=(Scale& lhs, float rhs);

/**
 * @brief Adds `rhs` to `lhs` component-wise.
 */
Scale& operator+=(Scale& lhs, Scale const& rhs);

/**
 * @brief Subtracts `rhs` from both components of `lhs`.
 */
Scale& operator-=(Scale& lhs, float rhs);

/**
 * @brief Subtracts `rhs` from `lhs` component-wise.
 */
Scale& operator-=(Scale& lhs, Scale const& rhs);

/**
 * @brief Multiplies both components of `lhs` by `rhs`.
 */
Scale& operator*=(Scale& lhs, float rhs);

/**
 * @brief Multiplies `lhs` by `rhs` component-wise.
 */
Scale& operator*=(Scale& lhs, Scale const& rhs);

/**
 * @brief Divides both components of `lhs` by `rhs`.
 */
Scale& operator/=(Scale& lhs, float rhs);

/**
 * @brief Divides `lhs` by `rhs` component-wise.
 */
Scale& operator/=(Scale& lhs, Scale const& rhs);
} // namespace kke
