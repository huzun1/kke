#include <charconv>
#include <cstdio>
#include <string_view>

#include "CachedShadowBenchmark.hh"

int main(int argc, char** argv) {
	std::size_t samples = 300;
	std::size_t rounds = 6;
	std::string_view comparison = argc == 4 ? argv[3] : "";
	if (argc > 4 || (!comparison.empty() && comparison != "bitmap" && comparison != "resolution")) {
		std::fprintf(
			stderr,
			"Usage: kke_cached_shadow_benchmark [samples=300] [rounds=6] [bitmap|resolution]\n"
		);
		return 1;
	}
	for (int index = 1; index < argc && index < 3; ++index) {
		std::string_view value(argv[index]);
		auto& destination = index == 1 ? samples : rounds;
		auto parsed = std::from_chars(value.data(), value.data() + value.size(), destination);
		if (parsed.ec != std::errc{} || parsed.ptr != value.data() + value.size() ||
			destination == 0 || destination > (index == 1 ? 10000u : 30u)) {
			std::fprintf(stderr, "samples must be 1..10000; rounds must be 1..30\n");
			return 1;
		}
	}
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
		return 1;
	}
	int result = kke::benchmark::CachedShadowBenchmark(samples, rounds, comparison).run() ? 0 : 1;
	CoUninitialize();
	return result;
}
