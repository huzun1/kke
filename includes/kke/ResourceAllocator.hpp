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

#include "CacheStorage.hpp"
#include "brush/Brush.hpp"
#include "kke/CacheStorage.hpp"
#include "kke/RenderSurface.hpp"
#include "kke/common/Geometry.hpp"
#include "kke/effect/Effect.hpp"
#include "kke/effect/EffectInstance.hpp"
#include "kke/font/FontLoader.hpp"
#include "kke/font/FontWeight.hpp"

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

	ComPtr<ID2D1Brush> aquireOrCreateBrush(kke::Brush const& brush);

	ComPtr<ID2D1Geometry> aquireOrCreateGeometry(kke::Geometry const& geometry);

	ComPtr<ID2D1Image> aquireOrDispatchShadow(kke::Geometry const& geometry, kke::Brush const& brush, float strength, std::function<void(ID2D1Image**)> dispatchFunc);

	IDWriteTextFormat* aquireOrCreateTextFormat(std::wstring const& fontFamily, int32_t fontSize, kke::FontWeight weight);

	IDWriteTextLayout* aquireOrCreateTextLayout(std::wstring const& text, IDWriteTextFormat* textFormat);

	EffectInstance* aquireOrCreateEffect(std::shared_ptr<Effect> effect);

	kke::RenderSurface* aquireOrCreateSurface();
};
};	// namespace kke
