#pragma once

namespace kke {
struct Color4f {
	float r, g, b, a;

	Color4f(float r, float g, float b);

	Color4f(float r, float g, float b, float a);

	Color4f(int hex);
};
};	// namespace kke
