#include "InvertedGeometryFactory.hh"

using namespace kke;
using namespace Microsoft::WRL;

ComPtr<ID2D1Geometry> GeometryInverter::create(
	D2dContext const& context,
	ID2D1Geometry* viewportGeometry,
	ID2D1Geometry* maskGeometry) {
	if (!viewportGeometry || !maskGeometry) {
		return nullptr;
	}

	ComPtr<ID2D1PathGeometry> invertedGeometry;
	HRESULT result = context.getFactory()->CreatePathGeometry(&invertedGeometry);
	if (FAILED(result)) {
		return nullptr;
	}

	ComPtr<ID2D1GeometrySink> sink;
	result = invertedGeometry->Open(&sink);
	if (FAILED(result)) {
		return nullptr;
	}

	result = viewportGeometry->CombineWithGeometry(
		maskGeometry,
		D2D1_COMBINE_MODE_EXCLUDE,
		nullptr,
		sink.Get());
	if (FAILED(result)) {
		return nullptr;
	}

	result = sink->Close();
	if (FAILED(result)) {
		return nullptr;
	}

	return invertedGeometry;
}
