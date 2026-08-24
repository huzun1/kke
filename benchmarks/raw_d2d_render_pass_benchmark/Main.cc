#include <charconv>
#include <cstddef>
#include <iostream>
#include <string_view>

#include "RawD2dRenderPassBenchmark.hh"

int main(int argumentCount, char** arguments) {
	size_t measuredFrameCount = 600;
	if (argumentCount > 1) {
		std::string_view value(arguments[1]);
		auto result =
			std::from_chars(value.data(), value.data() + value.size(), measuredFrameCount);
		if (result.ec != std::errc{} || result.ptr != value.data() + value.size() ||
			measuredFrameCount == 0) {
			std::cerr << "frame count must be a positive integer\n";
			return 1;
		}
	}

	try {
		return raw_d2d_benchmark::RawD2dRenderPassBenchmark(measuredFrameCount).run();
	} catch (std::exception const& exception) {
		std::cerr << "raw D2D benchmark failed: " << exception.what() << '\n';
		return 1;
	}
}
