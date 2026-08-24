#include "ViewLayerController.hh"

#include <cmath>
#include <variant>

#include "kke/appearance/view/LayerMode.hh"
#include "kke/utils/DebugLog.hh"

using Microsoft::WRL::ComPtr;

void ViewLayerController::pushLayer(
	kke::D2dEngineContext const& context, kke::MaskSource const& mask, kke::LayerMode mode
) {
	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	if (kke::Rect const* rect = axisAlignedClipRect(context, mask, mode)) {
		// Direct2D PushLayer creates an intermediate layer that is composited by PopLayer. That
		// cost is substantial when UI code clips many small rectangular regions. A normal Rect
		// under an axis-preserving transform has scissor semantics, so use the much cheaper
		// aliased axis-aligned clip internally. Inverted masks and rotated or skewed transforms
		// must retain a geometric layer because an axis-aligned clip cannot represent them.
		// The public KKE API remains a single pushLayer/popLayer stack; pushedLayerTypes records
		// which Direct2D pop operation corresponds to each entry.
		deviceContext->PushAxisAlignedClip(
			D2D1::RectF(rect->min.x, rect->min.y, rect->max.x, rect->max.y),
			D2D1_ANTIALIAS_MODE_ALIASED
		);
		pushedLayerTypes.push_back(PushedLayerType::AxisAlignedClip);
		return;
	}

	ComPtr<ID2D1Geometry> geometry = createGeometry(context, mask, mode);
	if (!geometry) {
		kke::debug::log("[kke][ViewLayerController] failed to create layer geometry");
		pushedLayerTypes.push_back(PushedLayerType::NoOperation);
		return;
	}

	deviceContext->PushLayer(D2D1::LayerParameters1(D2D1::InfiniteRect(), geometry.Get()), nullptr);
	pushedLayerTypes.push_back(PushedLayerType::Layer);
}

void ViewLayerController::popLayer(kke::D2dEngineContext const& context) {
	if (pushedLayerTypes.empty()) {
		kke::debug::log("[kke][ViewLayerController] popLayer called without a matching pushLayer");
		return;
	}

	PushedLayerType type = pushedLayerTypes.back();
	pushedLayerTypes.pop_back();
	ID2D1DeviceContext* deviceContext = context.getD2dContext()->getDeviceContext();
	switch (type) {
	case PushedLayerType::NoOperation:
		return;
	case PushedLayerType::AxisAlignedClip:
		deviceContext->PopAxisAlignedClip();
		return;
	case PushedLayerType::Layer:
		deviceContext->PopLayer();
		return;
	}
}

kke::Rect const* ViewLayerController::axisAlignedClipRect(
	kke::D2dEngineContext const& context, kke::MaskSource const& mask, kke::LayerMode mode
) {
	if (mode != kke::LayerMode::Normal) {
		return nullptr;
	}
	auto geometry = std::get_if<kke::Geometry>(&mask);
	if (geometry == nullptr) {
		return nullptr;
	}
	auto rect = std::get_if<kke::Rect>(geometry);
	if (rect == nullptr) {
		return nullptr;
	}

	D2D1_MATRIX_3X2_F transform;
	context.getD2dContext()->getDeviceContext()->GetTransform(&transform);
	constexpr float axisPreservingTolerance = 1.0e-6f;
	if (std::abs(transform._12) > axisPreservingTolerance ||
		std::abs(transform._21) > axisPreservingTolerance) {
		return nullptr;
	}
	return rect;
}

ComPtr<ID2D1Geometry> ViewLayerController::createGeometry(
	kke::D2dEngineContext const& context, kke::MaskSource const& mask, kke::LayerMode mode
) {
	// NOTE: If we will use kke::LayerMode in another place, we should close the switch statement to
	// another file
	switch (mode) {
	case kke::LayerMode::Normal:
		return std::visit(
			[&](auto const& maskVariant) {
				return context.getResourceProviders()->getGeometryProvider()->get(
					*context.getD2dContext(),
					maskVariant
				);
			},
			mask
		);
	case kke::LayerMode::Inverted:
		return std::visit(
			[&](auto const& maskVariant) {
				return context.getResourceProviders()->getInvertedGeometryProvider()->get(
					*context.getD2dContext(),
					*context.getResourceProviders()->getGeometryProvider(),
					maskVariant
				);
			},
			mask
		);
	default:
		kke::debug::log("[kke][ViewLayerController] unsupported layer mode");
		return nullptr;
	}
}
