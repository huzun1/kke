#include <kke/renderer/Color.hh>

using namespace kke;

Color4f::Color4f()
	: r(0.0f),
	  g(0.0f),
	  b(0.0f),
	  a(1.0f) {
}

Color4f::Color4f(float r, float g, float b)
	: r(r),
	  g(g),
	  b(b),
	  a(1.0f) {
}

Color4f::Color4f(float r, float g, float b, float a)
	: r(r),
	  g(g),
	  b(b),
	  a(a) {
}
