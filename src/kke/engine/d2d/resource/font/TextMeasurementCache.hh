#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <unordered_map>

#include "kke/appearance/Scale.hh"

namespace kke {
class TextMeasurementCache {
	struct CachedMeasurement {
		Scale size;
		uint64_t lastUsed;
	};

	size_t limit;
	uint64_t usageClock = 0;
	std::unordered_map<uint64_t, CachedMeasurement> measurementsByTextKey;

  public:
	explicit TextMeasurementCache(size_t limit = 2048);

	std::optional<Scale> get(uint64_t textKey);

	void put(uint64_t textKey, Scale size);

	void clear();

  private:
	void trim();

	uint64_t nextUsageStamp();
};
} // namespace kke
