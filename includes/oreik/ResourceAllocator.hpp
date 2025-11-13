#pragma once

#include <d2d1.h>
#include <d2d1_1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <wrl/client.h>

#include <functional>
#include <string>
#include <vector>

#include "CacheStorage.hpp"
#include "brush/Brush.hpp"
#include "oreik/CacheStorage.hpp"
#include "oreik/RenderSurface.hpp"
#include "oreik/common/Geometry.hpp"
#include "oreik/font/FontLoader.hpp"
#include "oreik/font/FontWeight.hpp"

namespace oreik {
class ResourceAllocator {
	ID2D1DeviceContext* deviceContext;
	FontLoader* fontLoader;

	CacheStorage<ID2D1Brush> brushStorage;
	CacheStorage<ID2D1Image> shadowStorage;
	CacheStorage<IDWriteTextFormat> textFormatStorage;
	CacheStorage<IDWriteTextLayout> textLayoutStorage;
	std::vector<RenderSurface> surfaces;

public:
	ResourceAllocator(ID2D1DeviceContext* context, FontLoader* fontLoader);

	Microsoft::WRL::ComPtr<ID2D1Brush> aquireOrCreateBrush(oreik::Brush const& brush);

	Microsoft::WRL::ComPtr<ID2D1Image> aquireOrDispatchShadow(oreik::Geometry const& geometry, oreik::Brush const& brush, float strength, std::function<void(ID2D1Image**)> dispatchFunc);

	oreik::RenderSurface* aquireOrCreateSurface(ID2D1DeviceContext* context);

	IDWriteTextFormat* aquireOrCreateTextFormat(std::wstring const& fontFamily, int32_t fontSize, oreik::FontWeight weight);

	IDWriteTextLayout* aquireOrCreateTextLayout(std::wstring const& text, IDWriteTextFormat* textFormat);
};
};	// namespace oreik
