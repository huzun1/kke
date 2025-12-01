#pragma once

#include <d2d1.h>
#include <d2d1_1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wrl/client.h>

#include <functional>
#include <future>
#include <string>
#include <vector>

#include "CacheStorage.hh"
#include "brush/Brush.hh"
#include "kke/CacheStorage.hh"
#include "kke/RenderSurface.hh"
#include "kke/common/Geometry.hh"
#include "kke/effect/Effect.hh"
#include "kke/effect/EffectInstance.hh"
#include "kke/font/FontLoader.hh"
#include "kke/font/FontWeight.hh"

using namespace Microsoft::WRL;

namespace kke {
class ResourceAllocator {
	ID2D1Factory* factory;
	ID2D1DeviceContext* deviceContext;
	FontLoader* fontLoader;

	CacheStorage<ID2D1Brush> brushStorage;
	CacheStorage<ID2D1Geometry> geometryStorage;
	CacheStorage<ID2D1Image> shadowStorage;
	CacheStorage<IDWriteTextFormat> textFormatStorage;
	CacheStorage<IDWriteTextLayout> textLayoutStorage;
	std::vector<EffectInstance> effectInstances;
	std::vector<RenderSurface> surfaces;

public:
	ResourceAllocator(ID2D1Factory* factory, ID2D1DeviceContext* context, FontLoader* fontLoader);

	void nextFrame();

	ComPtr<ID2D1Brush> acquireOrCreateBrush(kke::Brush const& brush);

	ComPtr<ID2D1Geometry> acquireOrCreateGeometry(kke::Geometry const& geometry);

	ComPtr<ID2D1Image> acquireOrDispatchShadow(kke::Geometry const& geometry, kke::Brush const& brush, float strength, std::function<void(ID2D1Image**)> dispatchFunc);

	ComPtr<ID2D1Image> acquireOrDispatchShadow(kke::Geometry const& geometry, uint64_t indentifierHash, kke::Brush const& brush, float strength, std::function<void(ID2D1Image**)> dispatchFunc);

	IDWriteTextFormat* acquireOrCreateTextFormat(std::wstring const& fontFamily, int32_t fontSize, kke::FontWeight weight);

	IDWriteTextLayout* acquireOrCreateTextLayout(std::wstring const& text, IDWriteTextFormat* textFormat);

	EffectInstance* acquireOrCreateEffect(std::shared_ptr<Effect> effect);

	kke::RenderSurface* acquireOrCreateSurface();
};
};	// namespace kke
