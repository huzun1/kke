#include <array>
#include <cstddef>
#include <memory>

#include "D2dTestTarget.hh"
#include "kke/appearance/resource/brush/impl/TextureBrush.hh"
#include "kke/appearance/resource/texture/RgbaBitmap.hh"
#include "kke/engine/d2d/renderer/effect/renderers/PositionIndependentEffectCache.hh"
#include "kke/engine/d2d/resource/texture/D2dTexture.hh"

namespace kke::test {
class TextureBrushEffectCacheTest {
	D2dTestTarget target;
	alignas(D2dTexture) std::array<std::byte, sizeof(D2dTexture)> textureStorage;

  public:
	bool run() {
		if (!target.initialize()) {
			return false;
		}
		for (bool isRed : {true, false, true}) {
			RgbaBitmap bitmap(8, 8);
			bitmap.fill(isRed ? 255 : 0, 0, isRed ? 0 : 255);
			auto uploaded = std::dynamic_pointer_cast<D2dTexture>(
				target.begin().uploadTexture(bitmap.rawTextureData())
			);
			if (!uploaded || target.finish().empty()) {
				return false;
			}
			auto texture = std::shared_ptr<D2dTexture>(
				std::construct_at(
					reinterpret_cast<D2dTexture*>(textureStorage.data()),
					uploaded->getBitmap()
				),
				[](D2dTexture* texture) { std::destroy_at(texture); }
			);
			for (bool repeated : {false, true}) {
				PositionIndependentEffectCache::consumeStats();
				auto& engine = target.begin();
				Rect destination{{16, 16}, {96, 96}};
				engine.renderEffect(
					Geometry(destination),
					{.brush = TextureBrush(texture, destination)},
					BlurEffect{}
				);
				auto pixels = target.finish();
				auto stats = PositionIndependentEffectCache::consumeStats();
				if (pixels.empty() || (repeated ? stats.hits != 1 : stats.misses != 1)) {
					return false;
				}
				size_t center = (48 * 256 + 48) * 4;
				if (pixels[center + (isRed ? 2 : 0)] < 240 ||
					pixels[center + (isRed ? 0 : 2)] > 10) {
					return false;
				}
			}
			std::weak_ptr<Texture> weak = texture;
			texture.reset();
			if (!weak.expired()) {
				return false;
			}
		}
		return true;
	}
};
} // namespace kke::test

int main() {
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
		return 1;
	}
	int result = kke::test::TextureBrushEffectCacheTest().run() ? 0 : 1;
	CoUninitialize();
	return result;
}
