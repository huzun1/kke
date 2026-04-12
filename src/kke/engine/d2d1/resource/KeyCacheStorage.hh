#pragma once

#include "kke/engine/d2d1/d2d1_headers.hh"

#include <algorithm>
#include <cstdint>
#include <unordered_map>

namespace kke {
template <typename T>
class KeyCacheStorage {
	using CacheKey = uint64_t;
	using UsageCount = uint32_t;
	using Ptr = Microsoft::WRL::ComPtr<T>;

	struct CachedPtr {
		UsageCount usageCount;
		Ptr ptr;
	};


	uint32_t limit;
	std::unordered_map<CacheKey, CachedPtr> storage;

public:
	KeyCacheStorage(uint32_t limit = UINT32_MAX)
		: limit(limit) {
	}

	/**
	 * @brief Store instances in the cache, and if the limit is exceeded, remove them starting with the lowest hit rate.
	 */
	void put(CacheKey key, Ptr val) {
		storage[key] = {0, val};
		clean();
	}

	/**
	 * @brief Attempting to retrieve an instance from the cache
	 */
	Ptr get(CacheKey key) {
		auto it = storage.find(key);
		if (it == storage.end()) {
			return nullptr;
		}
		it->second.usageCount++;
		return it->second.ptr;
	}

private:
	void clean() {
		if (storage.size() <= limit) {
			return;
		}
		
		auto usageList = getLessUsedKeys();

		size_t requiredToRemove = storage.size() - limit;
		for (size_t i = 0; i < requiredToRemove; ++i) {
			storage.erase(usageList[i].first);
		}
	}

	std::vector<std::pair<CacheKey, UsageCount>> getLessUsedKeys() {
		std::vector<std::pair<CacheKey, UsageCount>> usageList;
		usageList.reserve(storage.size());
		for (const auto& entry : storage) {
			usageList.emplace_back(entry.first, entry.second.usageCount);
		}

		std::sort(usageList.begin(), usageList.end(),
				  [](const auto& a, const auto& b) {
			return a.second < b.second;
		});

		return usageList;
	};
};
}  // namespace kke
