#include "Color.hh"

using namespace kke;

Color::Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {
}

Color::Color(float r, float g, float b) : r(r), g(g), b(b), a(1.0f) {
}

Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {
}
