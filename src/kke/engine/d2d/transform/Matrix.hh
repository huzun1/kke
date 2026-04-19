#pragma once

#include <memory>
#include <vector>

#include "Manipulator.hh"
#include "kke/appearance/transform/Scaling.hh"
#include "kke/appearance/transform/Translation.hh"
#include "kke/appearance/transform/Rotation.hh"

namespace kke {
class Matrix {
	std::vector<std::shared_ptr<Manipulator>> manipulators;

public:
    void pushTransform(Translation const& transform);

	void pushTransform(Scaling const& scale);

	void pushTransform(Rotation const& rotate);

	void popTransform();

	D2D1::Matrix3x2F build();
};
};	// namespace kke
