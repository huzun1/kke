#include "kke/common/geometry/Triangle.hh"

#include <wrl/client.h>

#include <cstdint>

#include "kke/common/Geometry.hh"
#include "kke/common/Point.hh"
#include "kke/internal/Hasher.hh"

using namespace kke;
using namespace Microsoft::WRL;

Triangle::Triangle()
	: a(kke::Point2f{0.0f, 0.0f}),
	  b(kke::Point2f{0.0f, 0.0f}),
	  c(kke::Point2f{0.0f, 0.0f}) {
}

Triangle::Triangle(kke::Point2f const& a, kke::Point2f const& b, kke::Point2f const& c)
	: a(a), b(b), c(c) {
}

void Triangle::create(ID2D1Factory* factory, ID2D1Geometry** output) const {
	ID2D1PathGeometry* geometry;

	factory->CreatePathGeometry(&geometry);
	ComPtr<ID2D1GeometrySink> sink;
	geometry->Open(&sink);
	sink->BeginFigure(a.point2f(), D2D1_FIGURE_BEGIN_FILLED);
	sink->AddLine(b.point2f());
	sink->AddLine(c.point2f());
	sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	sink->Close();

	*output = geometry;
}

uint64_t Triangle::hash(bool positionDependent) const {
	Hasher hasher;
	hasher.combine(static_cast<uint32_t>(GeometryType::TRIANGLE));
	hasher.combine(positionDependent);
	if (positionDependent) {
		hasher.combine(a.hash());
		hasher.combine(b.hash());
		hasher.combine(c.hash());
	} else {
		// Invert the triangle points to make the hash position independent
		kke::Point2f invertVec = {-1.0f, -1.0f};
		hasher.combine(b.offset(a * invertVec).hash());
		hasher.combine(c.offset(a * invertVec).hash());
	}
	return hasher.get();
}
