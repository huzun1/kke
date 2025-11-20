#pragma once

#include <d2d1.h>
#include <d2d1_1.h>
#include <wincodec.h>
#include <wrl/client.h>

#include <cstddef>
#include <unordered_map>

using namespace Microsoft::WRL;

namespace kke {
typedef std::size_t TextureId;
class TextureRepository {
	ID2D1DeviceContext* deviceContext;
	std::unordered_map<TextureId, ComPtr<ID2D1Bitmap>> textures;

	TextureId nextId = 0;

public:
	TextureRepository(ID2D1DeviceContext* context);

	ComPtr<ID2D1Bitmap> getTexture(TextureId id);

	TextureId load(const void* data, size_t size);

	bool exists(TextureId id) const;

	void release(TextureId id);

private:
	TextureId assignTexId();
};
};	// namespace kke
