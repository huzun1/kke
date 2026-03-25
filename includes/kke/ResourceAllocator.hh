#pragma once

#include <d2d1.h>
#include <d2d1_1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wrl/client.h>

#include <cstddef>
#include <functional>
#include <memory>
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

namespace kke {
class ResourceAllocator {
	ID2D1Factory* factory;
	ID2D1DeviceContext* deviceContext;
	FontLoader* fontLoader;

	CacheStorage<ID2D1Brush> brushStorage;
	CacheStorage<ID2D1Geometry> geometryStorage;
	CacheStorage<ID2D1Geometry> invertedGeometryStorage;
	CacheStorage<ID2D1Image> shadowStorage;
	CacheStorage<IDWriteTextFormat> textFormatStorage;
	CacheStorage<IDWriteTextLayout> textLayoutStorage;
	std::vector<std::shared_ptr<EffectInstance>> effectInstances;
	std::vector<std::shared_ptr<RenderSurface>> surfaces;

	static constexpr size_t kEffectInstanceLimit = 16;
	static constexpr size_t kSurfaceLimit = 8;

public:
	ResourceAllocator(ID2D1Factory* factory, ID2D1DeviceContext* context, FontLoader* fontLoader);

	void nextFrame();

	Microsoft::WRL::ComPtr<ID2D1Brush> acquireOrCreateBrush(kke::Brush const& brush);

	Microsoft::WRL::ComPtr<ID2D1Geometry> acquireOrCreateGeometry(kke::Geometry const& geometry);

	Microsoft::WRL::ComPtr<ID2D1Geometry> acquireOrCreateInvertedGeometry(kke::Geometry const& geometry);

	Microsoft::WRL::ComPtr<ID2D1Image> acquireOrDispatchShadow(kke::Geometry const& geometry, kke::Brush const& brush, float strength, std::function<void(ID2D1Image**)> dispatchFunc);

	Microsoft::WRL::ComPtr<ID2D1Image> acquireOrDispatchShadow(kke::Geometry const& geometry, uint64_t identifierHash, kke::Brush const& brush, float strength, std::function<void(ID2D1Image**)> dispatchFunc);

	IDWriteTextFormat* acquireOrCreateTextFormat(std::wstring const& fontFamily, int32_t fontSize, kke::FontWeight weight);

	IDWriteTextLayout* acquireOrCreateTextLayout(std::wstring const& text, IDWriteTextFormat* textFormat);

	EffectInstance* acquireOrCreateEffect(std::shared_ptr<Effect> effect);

	kke::RenderSurface* acquireOrCreateSurface();

private:
	void trimEffectInstances();

	void trimSurfaces();
};
};	// namespace kke
