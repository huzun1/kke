#include <cstdio>
#include <cstdlib>

#include "D2dTestTarget.hh"
#include "kke/appearance/resource/brush/impl/SolidColorBrush.hh"
#include "kke/engine/d2d/renderer/effect/renderers/PositionIndependentEffectCache.hh"

namespace kke::test {
class ShadowBitmapDrawingTest {
	D2dTestTarget target;

  public:
	bool run() {
		if (!target.initialize()) {
			return false;
		}
		for (bool hasExplicitClip : {false, true}) {
			for (bool usesCanvas : {false, true}) {
				for (int frame = 0; frame < 2; ++frame) {
					if (!verifyFrame(hasExplicitClip, usesCanvas, frame)) {
						std::fprintf(
							stderr,
							"Shadow composition failed: clip=%d canvas=%d frame=%d\n",
							hasExplicitClip,
							usesCanvas,
							frame
						);
						return false;
					}
				}
			}
		}
		return true;
	}

  private:
	bool verifyFrame(bool hasExplicitClip, bool usesCanvas, int frame) {
		PositionIndependentEffectCache::consumeStats();
		auto& engine = target.begin();
		float x = 32.0f + frame * 16.0f;
		Rect shape{{x, 32}, {x + 64, 96}};
		EffectSource source = Geometry(shape);
		if (usesCanvas) {
			auto canvas = engine.createCanvas();
			if (!canvas || !engine.beginCanvas(canvas)) {
				return false;
			}
			engine.fill(shape, SolidColorBrush({1, 1, 1, 1}));
			if (!engine.endCanvas() || !engine.finishCanvas(canvas)) {
				return false;
			}
			source = canvas;
		}
		engine.fill(Rect{{0, 0}, {256, 256}}, SolidColorBrush({0, 0, 1, 1}));
		std::optional<EffectClipSource> clip;
		if (hasExplicitClip) {
			clip = Geometry(Rect{{x, 32}, {x + 32, 96}});
		}
		engine.renderEffect(
			source,
			{},
			ShadowEffect{
				.offset = {0, 0},
				.blurStandardDeviation = 0,
				.color = {1, 0, 0, 0.5f},
				.mode = ShadowMode::ShadowOnly
			},
			clip
		);
		auto pixels = target.finish();
		auto stats = PositionIndependentEffectCache::consumeStats();
		if (pixels.empty() ||
			(usesCanvas ? stats.hits != 0 || stats.misses != 0
						: stats.hits + stats.misses != 1 || (frame == 1 && stats.hits != 1))) {
			return false;
		}
		return matchesPixel(
				   pixels,
				   int(x) + 16,
				   64,
				   hasExplicitClip ? 0 : 127,
				   0,
				   128,
				   hasExplicitClip ? 128 : 255
			   ) &&
			   matchesPixel(
				   pixels,
				   int(x) + 48,
				   64,
				   hasExplicitClip ? 255 : 127,
				   0,
				   hasExplicitClip ? 0 : 128,
				   255
			   ) &&
			   matchesPixel(pixels, 200, 200, 255, 0, 0, 255);
	}

	static bool matchesPixel(
		std::vector<uint8_t> const& pixels, int x, int y, int blue, int green, int red, int alpha
	) {
		std::size_t offset = (std::size_t(y) * 256 + x) * 4;
		return std::abs(int(pixels[offset]) - blue) <= 1 &&
			   std::abs(int(pixels[offset + 1]) - green) <= 1 &&
			   std::abs(int(pixels[offset + 2]) - red) <= 1 &&
			   std::abs(int(pixels[offset + 3]) - alpha) <= 1;
	}
};
} // namespace kke::test

int main() {
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
		return 1;
	}
	int result = kke::test::ShadowBitmapDrawingTest().run() ? 0 : 1;
	CoUninitialize();
	return result;
}
