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
