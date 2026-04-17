#pragma once

#define KKE_COLOR_24(hex) kke::Color( \
	((hex >> 24) & 0xFF) / 255.0f,      \
	((hex >> 16) & 0xFF) / 255.0f,      \
	((hex >> 8) & 0xFF) / 255.0f,       \
	(hex & 0xFF) / 255.0f)

#define KKE_COLOR_16(hex) kke::Color( \
	((hex >> 16) & 0xFF) / 255.0f,      \
	((hex >> 8) & 0xFF) / 255.0f,       \
	(hex & 0xFF) / 255.0f)

namespace kke {
struct Color {
	float r, g, b, a;

	Color();

	Color(float r, float g, float b);

	Color(float r, float g, float b, float a);
};
};	// namespace kke
