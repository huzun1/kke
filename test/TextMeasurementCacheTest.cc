#include "kke/engine/d2d/resource/font/TextMeasurementCache.hh"

int main() {
	kke::TextMeasurementCache cache(2);

	cache.put(1, {10.0f, 20.0f});
	cache.put(2, {30.0f, 40.0f});
	if (!cache.get(1).has_value()) {
		return 1;
	}

	cache.put(3, {50.0f, 60.0f});
	if (cache.get(2).has_value()) {
		return 1;
	}
	auto retainedSize = cache.get(1);
	if (!retainedSize.has_value() || retainedSize->x != 10.0f || retainedSize->y != 20.0f) {
		return 1;
	}

	cache.clear();
	if (cache.get(1).has_value() || cache.get(3).has_value()) {
		return 1;
	}

	kke::TextMeasurementCache disabledCache(0);
	disabledCache.put(1, {10.0f, 20.0f});
	if (disabledCache.get(1).has_value()) {
		return 1;
	}
}
