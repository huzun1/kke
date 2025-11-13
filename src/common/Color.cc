#include <kke/common/Color.hpp>

kke::Color4f::Color4f(float r, float g, float b)
	: r(r),
	  g(g),
	  b(b),
	  a(1.0f) {
}

kke::Color4f::Color4f(float r, float g, float b, float a)
	: r(r),
	  g(g),
	  b(b),
	  a(a) {
}

kke::Color4f::Color4f(int hex) {
	r = (hex >> 16 & 0xFF) / 255.0f;
	g = (hex >> 8 & 0xFF) / 255.0f;
	b = (hex & 0xFF) / 255.0f;
	a = 1.0f;
};
