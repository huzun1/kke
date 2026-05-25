#include "ViewportGeometryCache.hh"

using namespace kke;
using namespace Microsoft::WRL;

void ViewportGeometryCache::syncViewportSize(D2dContext const& context) {
	D2D1_SIZE_F viewportSize = context.getDeviceContext()->GetSize();
	if (!isViewportResized(viewportSize)) {
		return;
	}

	viewportGeometry = nullptr;
	viewportGeometrySize = viewportSize;
}

ComPtr<ID2D1RectangleGeometry> ViewportGeometryCache::get(D2dContext const& context) {
	if (viewportGeometry) {
		return viewportGeometry;
	}

	D2D1_SIZE_F viewportSize = context.getDeviceContext()->GetSize();
	D2D1_RECT_F viewportRect = {0.0f, 0.0f, viewportSize.width, viewportSize.height};

	ComPtr<ID2D1RectangleGeometry> createdGeometry;
	HRESULT result = context.getFactory()->CreateRectangleGeometry(viewportRect, &createdGeometry);
	if (FAILED(result)) {
		return nullptr;
	}

	viewportGeometry = createdGeometry;
	viewportGeometrySize = viewportSize;
	return viewportGeometry;
}

bool ViewportGeometryCache::isViewportResized(D2D1_SIZE_F viewportSize) const {
	return viewportGeometrySize.width != viewportSize.width
		|| viewportGeometrySize.height != viewportSize.height;
}
