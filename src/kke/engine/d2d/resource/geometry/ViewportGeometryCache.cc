#include "ViewportGeometryCache.hh"

#include <cstdio>

using namespace kke;
using namespace Microsoft::WRL;

void ViewportGeometryCache::syncViewportSize(D2dContext const& context, D2D1_SIZE_F viewportSize) {
	if (!isViewportResized(viewportSize)) {
		return;
	}

	std::printf("[kke][ViewportGeometryCache] viewport resized: %.2f x %.2f\n", viewportSize.width, viewportSize.height);
	viewportGeometry = createViewportGeometry(context, viewportSize);
	viewportGeometrySize = viewportSize;
}

ComPtr<ID2D1RectangleGeometry> ViewportGeometryCache::get(D2dContext const& context) {
	if (viewportGeometry) {
		return viewportGeometry;
	}

	D2D1_SIZE_F viewportSize = context.getDeviceContext()->GetSize();
	viewportGeometry = createViewportGeometry(context, viewportSize);
	viewportGeometrySize = viewportSize;
	return viewportGeometry;
}

ComPtr<ID2D1RectangleGeometry> ViewportGeometryCache::createViewportGeometry(
	D2dContext const& context,
	D2D1_SIZE_F viewportSize) const {
	if (viewportSize.width <= 0.0f || viewportSize.height <= 0.0f) {
		std::printf("[kke][ViewportGeometryCache] invalid viewport size: %.2f x %.2f\n", viewportSize.width, viewportSize.height);
	}

	D2D1_RECT_F viewportRect = {0.0f, 0.0f, viewportSize.width, viewportSize.height};

	ComPtr<ID2D1RectangleGeometry> createdGeometry;
	HRESULT result = context.getFactory()->CreateRectangleGeometry(viewportRect, &createdGeometry);
	if (FAILED(result)) {
		std::printf("[kke][ViewportGeometryCache] CreateRectangleGeometry failed: 0x%08lx\n", static_cast<unsigned long>(result));
		return nullptr;
	}

	return createdGeometry;
}

bool ViewportGeometryCache::isViewportResized(D2D1_SIZE_F viewportSize) const {
	return viewportGeometrySize.width != viewportSize.width
		|| viewportGeometrySize.height != viewportSize.height;
}
