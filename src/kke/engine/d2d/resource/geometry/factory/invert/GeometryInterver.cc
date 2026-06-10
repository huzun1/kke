#include "GeometryInverter.hh"

#include "kke/utils/DebugLog.hh"

using namespace kke;
using namespace Microsoft::WRL;

ComPtr<ID2D1Geometry> GeometryInverter::create(
	D2dContext const& context, ID2D1Geometry* viewportGeometry, ID2D1Geometry* maskGeometry
) {
	if (!viewportGeometry || !maskGeometry) {
		kke::debug::log(
			"[kke][GeometryInverter] null input: viewport=%p mask=%p",
			static_cast<void*>(viewportGeometry),
			static_cast<void*>(maskGeometry)
		);
		return nullptr;
	}

	ComPtr<ID2D1PathGeometry> invertedGeometry;
	HRESULT result = context.getFactory()->CreatePathGeometry(&invertedGeometry);
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][GeometryInverter] CreatePathGeometry failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
		return nullptr;
	}

	ComPtr<ID2D1GeometrySink> sink;
	result = invertedGeometry->Open(&sink);
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][GeometryInverter] Open sink failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
		return nullptr;
	}

	result =
		viewportGeometry
			->CombineWithGeometry(maskGeometry, D2D1_COMBINE_MODE_EXCLUDE, nullptr, sink.Get());
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][GeometryInverter] CombineWithGeometry failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
		return nullptr;
	}

	result = sink->Close();
	if (FAILED(result)) {
		kke::debug::log(
			"[kke][GeometryInverter] Close sink failed: 0x%08x",
			static_cast<unsigned int>(result)
		);
		return nullptr;
	}

	return invertedGeometry;
}
