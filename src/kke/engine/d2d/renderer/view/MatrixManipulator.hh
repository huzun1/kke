#pragma once

#include "kke/geometry/primitives/Point.hh"

class MatrixManipulator {
public:
	void pushTranslate(kke::Point2f const& offset);

	void pushScale(kke::Point2f const& center, kke::Scale2f const& scale);

	void pushRotate(kke::Point2f const& center, float angle);

	void popTranslate();

	void popScale();

	void popRotate();

private:
	void popTransform();
};
