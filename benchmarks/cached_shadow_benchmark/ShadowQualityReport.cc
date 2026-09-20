#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <string>

#include "ShadowQualityReport.hh"

using namespace kke::benchmark;

bool ShadowQualityReport::write(
	std::span<uint8_t const> reference,
	std::span<uint8_t const> pixels,
	std::string_view scene,
	std::string_view mode
) {
	if (reference.size() != 1920 * 1080 * 4 || pixels.size() != reference.size()) {
		return false;
	}
	std::string filename =
		"shadow-quality-" + std::string(scene) + "-" + std::string(mode) + ".ppm";
	std::ofstream output(filename, std::ios::binary);
	if (!output) {
		return false;
	}
	constexpr int width = 320;
	constexpr int height = 160;
	output << "P6\n" << width << ' ' << height << "\n255\n";
	int maximumDifference = 0;
	double absoluteDifference = 0;
	std::size_t changedPixels = 0;
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			std::size_t offset = (std::size_t(y) * 1920 + x) * 4;
			bool isChanged = false;
			for (int channel = 0; channel < 4; ++channel) {
				int difference =
					std::abs(int(reference[offset + channel]) - int(pixels[offset + channel]));
				maximumDifference = (std::max)(maximumDifference, difference);
				absoluteDifference += difference;
				isChanged |= difference != 0;
			}
			changedPixels += isChanged;
			for (int channel : {2, 1, 0}) {
				int composed = pixels[offset + channel] + (255 - pixels[offset + 3]) * 128 / 255;
				output.put(static_cast<char>((std::min)(255, composed)));
			}
		}
	}
	output.close();
	std::fprintf(
		stderr,
		"quality scene=%.*s mode=%.*s max_channel_delta=%d roi_mean_channel_delta=%.6f "
		"changed_pixels=%zu/%d file=%s\n",
		int(scene.size()),
		scene.data(),
		int(mode.size()),
		mode.data(),
		maximumDifference,
		absoluteDifference / (width * height * 4),
		changedPixels,
		width * height,
		filename.c_str()
	);
	return bool(output);
}
