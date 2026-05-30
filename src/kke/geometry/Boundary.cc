#include "Boundary.hh"

Boundary::Boundary() : top(0), left(0), right(0), bottom(0) {
}

Boundary::Boundary(float top, float left, float right, float bottom)
	: top(top), left(left), right(right), bottom(bottom) {
}

float Boundary::width() const {
    return right - left;
}

float Boundary::height() const {
    return bottom - top;
}

Boundary Boundary::shrink(float amount) {
    return Boundary(top + amount, left + amount, right - amount, bottom - amount);
}

Boundary Boundary::expand(float amount) {
    return Boundary(top - amount, left - amount, right + amount, bottom + amount);
}
