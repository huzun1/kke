#include "GeometryInverter.hh"

#include <cstdio>

using namespace kke;
using namespace Microsoft::WRL;

ComPtr<ID2D1Geometry> GeometryInverter::create(
	D2dContext const& context,
	ID2D1Geometry* viewportGeometry,
	ID2D1Geometry* maskGeometry) {
	if (!viewportGeometry || !maskGeometry) {
		std::printf("[kke][GeometryInverter] null input: viewport=%p mask=%p\n", viewportGeometry, maskGeometry);
		return nullptr;
	}

	ComPtr<ID2D1PathGeometry> invertedGeometry;
	HRESULT result = context.getFactory()->CreatePathGeometry(&invertedGeometry);
	if (FAILED(result)) {
		std::printf("[kke][GeometryInverter] CreatePathGeometry failed: 0x%08lx\n", static_cast<unsigned long>(result));
		return nullptr;
	}

	ComPtr<ID2D1GeometrySink> sink;
	result = invertedGeometry->Open(&sink);
	if (FAILED(result)) {
		std::printf("[kke][GeometryInverter] Open sink failed: 0x%08lx\n", static_cast<unsigned long>(result));
		return nullptr;
	}

	result = viewportGeometry->CombineWithGeometry(
		maskGeometry,
		D2D1_COMBINE_MODE_EXCLUDE,
		nullptr,
		sink.Get());
	if (FAILED(result)) {
		std::printf("[kke][GeometryInverter] CombineWithGeometry failed: 0x%08lx\n", static_cast<unsigned long>(result));
		return nullptr;
	}

	result = sink->Close();
	if (FAILED(result)) {
		std::printf("[kke][GeometryInverter] Close sink failed: 0x%08lx\n", static_cast<unsigned long>(result));
		return nullptr;
	}

	return invertedGeometry;
}
