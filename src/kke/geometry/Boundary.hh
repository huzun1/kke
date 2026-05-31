#pragma once

struct Boundary {
	float top, left, right, bottom;

	Boundary();

	Boundary(float top, float left, float right, float bottom);

	float width() const;

	float height() const;

	Boundary shrink(float amount);

	Boundary expand(float amount);
};