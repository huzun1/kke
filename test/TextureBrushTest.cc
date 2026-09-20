#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>

#include "D2dTestTarget.hh"
#include "kke/appearance/resource/brush/impl/TextureBrush.hh"
#include "kke/appearance/resource/texture/RgbaBitmap.hh"
#include "kke/engine/d2d/renderer/effect/cache/PositionIndependentBrush.hh"
#include "kke/engine/d2d/resource/brush/hash/BrushHasher.hh"
#include "kke/geometry/curved/RoundedRect.hh"

namespace kke::test {
class TextureBrushTest {
	D2dTestTarget target;
	std::shared_ptr<Texture> texture;

  public:
	bool run(bool benchmark) {
		if (!target.initialize() || !upload() || !hashAndNormalization() || !pixels()) {
			return false;
		}
		if (benchmark) {
			measure();
		}
		return true;
	}

  private:
	bool upload() {
		RgbaBitmap bitmap(16, 16);
		for (uint32_t y = 0; y < 16; ++y) {
			for (uint32_t x = 0; x < 16; ++x) {
				bitmap
					.setPixel(x, y, static_cast<uint8_t>(x * 16), static_cast<uint8_t>(y * 16), 96);
			}
		}
		texture = target.begin().uploadTexture(bitmap.rawTextureData());
		return !target.finish().empty() && texture != nullptr;
	}

	bool hashAndNormalization() {
		Rect destination{{20, 30}, {100, 90}};
		TextureDrawAppearance appearance{
			.opacity = 0.5f,
			.interpolation = TextureInterpolation::HighQualityCubic,
			.srcRect = Rect{{4, 2}, {12, 14}}
		};
		TextureBrush brush(texture, destination, appearance);
		auto normalized =
			std::get<TextureBrush>(PositionIndependentBrush::normalize(brush, {20, 30}));
		if (normalized.getTexture() != texture || normalized.getDestination().min.x != 0 ||
			normalized.getDestination().max.y != 60 ||
			normalized.getAppearance().srcRect->min.x != 4 ||
			normalized.getAppearance().opacity != 0.5f ||
			normalized.getAppearance().interpolation != TextureInterpolation::HighQualityCubic) {
			return false;
		}
		auto original = BrushHasher::hash(Brush(brush));
		for (int change = 0; change < 5; ++change) {
			auto modified = appearance;
			auto changedDestination = destination;
			switch (change) {
			case 0:
				modified.opacity = 0.6f;
				break;
			case 1:
				modified.interpolation = TextureInterpolation::Nearest;
				break;
			case 2:
				modified.srcRect->min.x = 5;
				break;
			case 3:
				modified.srcRect.reset();
				break;
			case 4:
				changedDestination.min.x = 21;
				break;
			}
			if (original ==
				BrushHasher::hash(Brush(TextureBrush(texture, changedDestination, modified)))) {
				return false;
			}
		}
		return original != BrushHasher::hash(Brush(TextureBrush(nullptr, destination, appearance)));
	}

	std::vector<uint8_t>
	render(bool layer, TextureInterpolation interpolation, bool transformed, int ancestorClip) {
		auto& engine = target.begin();
		if (ancestorClip) {
			if (ancestorClip == 1) {
				engine.pushLayer(Rect{{0, 0}, {64, 256}});
			} else {
				engine.pushLayer(RoundedRect({{0, 0}, {64, 256}}, 20));
			}
		}
		if (transformed) {
			engine.pushTransform(
				AffineTransform{.xx = 1.1f, .xy = -0.4f, .yx = 0.4f, .yy = 0.9f, .tx = 50, .ty = 5}
			);
		}
		Rect destination{{16, 16}, {112, 96}};
		RoundedRect geometry(destination, 18);
		TextureDrawAppearance appearance{
			.opacity = 0.65f,
			.interpolation = interpolation,
			.srcRect = Rect{{4, 2}, {12, 14}}
		};
		if (layer) {
			engine.pushLayer(geometry);
			engine.draw(texture, destination, appearance);
			engine.popLayer();
		} else {
			engine.fill(geometry, TextureBrush(texture, destination, appearance));
		}
		if (transformed) {
			engine.popTransform();
		}
		if (ancestorClip) {
			engine.popLayer();
		}
		return target.finish();
	}

	bool pixels() {
		for (auto interpolation :
			 {TextureInterpolation::Nearest,
			  TextureInterpolation::Linear,
			  TextureInterpolation::HighQualityCubic}) {
			for (bool transformed : {false, true}) {
				for (int ancestor : {0, 1, 2}) {
					auto reference = render(true, interpolation, transformed, ancestor);
					auto actual = render(false, interpolation, transformed, ancestor);
					if (reference.empty() || actual.empty() ||
						target.statistics().geometricLayerCount != (ancestor == 2 ? 1u : 0u)) {
						return false;
					}
					double error = 0;
					size_t covered = 0;
					for (size_t pixel = 0; pixel < actual.size(); pixel += 4) {
						if (actual[pixel + 3] || reference[pixel + 3]) {
							++covered;
							for (size_t channel = 0; channel < 4; ++channel) {
								error += std::abs(
									int(actual[pixel + channel]) - int(reference[pixel + channel])
								);
							}
						}
					}
					std::printf(
						"sampling=%d transformed=%d ancestor=%d covered=%zu mean_error=%.3f\n",
						int(interpolation),
						transformed,
						ancestor,
						covered,
						error / (4 * (std::max)(covered, size_t(1)))
					);
					if (covered < 1000 || error / (4 * covered) > 3.0) {
						return false;
					}
					if (!transformed && (actual[(16 * 256 + 16) * 4 + 3] != 0 ||
										 actual[(50 * 256 + 40) * 4 + 3] < 160 ||
										 (ancestor && actual[(50 * 256 + 80) * 4 + 3] != 0))) {
						return false;
					}
				}
			}
		}
		return true;
	}

	void measure() {
		for (bool layer : {true, false}) {
			std::vector<double> milliseconds;
			for (int frame = 0; frame < 140; ++frame) {
				auto start = std::chrono::steady_clock::now();
				auto& engine = target.begin();
				for (int index = 0; index < 64; ++index) {
					float x = float(index % 8) * 32;
					float y = float(index / 8) * 32;
					Rect destination{{x, y}, {x + 28, y + 28}};
					RoundedRect shape(destination, 7);
					TextureDrawAppearance appearance{
						.interpolation = TextureInterpolation::HighQualityCubic
					};
					if (layer) {
						engine.pushLayer(shape);
						engine.draw(texture, destination, appearance);
						engine.popLayer();
					} else {
						engine.fill(shape, TextureBrush(texture, destination, appearance));
					}
				}
				auto image = target.finish();
				if (frame >= 20) {
					milliseconds.push_back(std::chrono::duration<double, std::milli>(
											   std::chrono::steady_clock::now() - start
					)
											   .count());
				}
			}
			std::sort(milliseconds.begin(), milliseconds.end());
			std::printf(
				"64 images, layer=%d: median=%.3fms p95=%.3fms geometric_layers=%zu (includes "
				"synchronous readback)\n",
				layer,
				milliseconds[60],
				milliseconds[114],
				target.statistics().geometricLayerCount
			);
		}
	}
};
} // namespace kke::test

int main(int argc, char**) {
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
		return 1;
	}
	int result = kke::test::TextureBrushTest().run(argc > 1) ? 0 : 1;
	CoUninitialize();
	return result;
}
