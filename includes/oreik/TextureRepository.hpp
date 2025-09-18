#pragma once

#include <d2d1.h>
#include <d2d1_1.h>
#include <wrl/client.h>

#include <cstddef>
#include <vector>

namespace oreik {
class TextureRepository {
	ID2D1DeviceContext* deviceContext;

	std::vector<Microsoft::WRL::ComPtr<ID2D1Bitmap>> textures;

public:
	TextureRepository(ID2D1DeviceContext* context);

	Microsoft::WRL::ComPtr<ID2D1Bitmap> fetchTexture(size_t index);

	size_t load(const void* data, size_t size);
};
};	// namespace oreik
