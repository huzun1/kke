#include <oreik/common/Rect.hpp>

void oreik::Rect::offset(float x1, float y1, float x2, float y2) {
	this->x1 += x1;
	this->y1 += y1;
	this->x2 += x2;
	this->y2 += y2;
}

D2D1_RECT_F oreik::Rect::rectF() const {
	return {x1, y1, x2, y2};
}
