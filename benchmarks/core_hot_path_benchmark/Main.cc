#include <charconv>
#include <cstddef>
#include <iostream>
#include <string_view>

#include "CoreHotPathBenchmark.hh"

int main(int argumentCount, char** arguments) {
	size_t measuredIterationCount = 10000;
	if (argumentCount > 1) {
		std::string_view value(arguments[1]);
		auto result =
			std::from_chars(value.data(), value.data() + value.size(), measuredIterationCount);
		if (result.ec != std::errc{} || result.ptr != value.data() + value.size() ||
			measuredIterationCount == 0) {
			std::cerr << "iteration count must be a positive integer\n";
			return 1;
		}
	}

	return kke::benchmark::CoreHotPathBenchmark(measuredIterationCount).run();
}
