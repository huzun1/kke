#include "TextMeasurementCache.hh"

#include <algorithm>

using namespace kke;

TextMeasurementCache::TextMeasurementCache(size_t limit) : limit(limit) {
}

std::optional<Scale> TextMeasurementCache::get(uint64_t textKey) {
	auto measurement = measurementsByTextKey.find(textKey);
	if (measurement == measurementsByTextKey.end()) {
		return std::nullopt;
	}

	measurement->second.lastUsed = nextUsageStamp();
	return measurement->second.size;
}

void TextMeasurementCache::put(uint64_t textKey, Scale size) {
	if (limit == 0) {
		return;
	}

	measurementsByTextKey.insert_or_assign(
		textKey,
		CachedMeasurement{
			.size = size,
			.lastUsed = nextUsageStamp(),
		}
	);
	trim();
}

void TextMeasurementCache::clear() {
	measurementsByTextKey.clear();
	usageClock = 0;
}

void TextMeasurementCache::trim() {
	while (measurementsByTextKey.size() > limit) {
		auto leastRecentlyUsed = std::min_element(
			measurementsByTextKey.begin(),
			measurementsByTextKey.end(),
			[](auto const& left, auto const& right) {
				return left.second.lastUsed < right.second.lastUsed;
			}
		);
		measurementsByTextKey.erase(leastRecentlyUsed);
	}
}

uint64_t TextMeasurementCache::nextUsageStamp() {
	return ++usageClock;
}
