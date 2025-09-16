#pragma once

#include <unknwnbase.h>

#include <algorithm>
#include <cstdint>
#include <optional>
#include <unordered_map>

namespace oreik {
struct Cache {
	uint32_t usageCount;
	IUnknown* ptr;
};

template <typename T>
class CacheStorage {
	uint32_t limit;
	std::unordered_map<uint64_t, Cache> storage;

public:
	CacheStorage(uint32_t limit = 1000)
		: limit(limit) {
	}

	/**
	 * @brief Store instances in the cache, and if the limit is exceeded, remove them starting with the lowest hit rate.
	 */
	void put(uint64_t key, T* val) {
		storage[key] = {0, val};
		clean();
	}

	/**
	 * @brief Attempting to retrieve an instance from the cache
	 */
	std::optional<T*> get(uint64_t key) {
		auto it = storage.find(key);
		if (it == storage.end()) {
			return std::nullopt;
		}
		it->second.usageCount++;
		return static_cast<T*>(it->second.ptr);
	}

private:
	void clean() {
		if (storage.size() <= limit) {
			return;
		}

		// key, usageCount
		std::vector<std::pair<uint64_t, uint32_t>> usageList;
		usageList.reserve(storage.size());
		for (const auto& entry : storage) {
			usageList.emplace_back(entry.first, entry.second.usageCount);
		}
		std::sort(usageList.begin(), usageList.end(),
				  [](const auto& a, const auto& b) {
			return a.second < b.second;
		});

		size_t toRemove = storage.size() - limit;
		for (size_t i = 0; i < toRemove; ++i) {
			storage[usageList[i].first].ptr->Release();
			storage.erase(usageList[i].first);
		}
	}
};
}  // namespace oreik
