#pragma once

#include <unknwnbase.h>
#include <wrl/client.h>

#include <algorithm>
#include <cstdint>
#include <unordered_map>

namespace kke {
template <typename T>
class CacheStorage {
	typedef Microsoft::WRL::ComPtr<T> Ptr_t;

	struct Cache {
		uint32_t usageCount;
		Ptr_t ptr;
	};

	uint32_t limit;
	std::unordered_map<uint64_t, Cache> storage;

public:
	CacheStorage(uint32_t limit = 1000)
		: limit(limit) {
	}

	/**
	 * @brief Store instances in the cache, and if the limit is exceeded, remove them starting with the lowest hit rate.
	 */
	void put(uint64_t key, Ptr_t val) {
		storage[key] = {0, val};
		clean();
	}

	/**
	 * @brief Attempting to retrieve an instance from the cache
	 */
	Ptr_t get(uint64_t key) {
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
			storage.erase(usageList[i].first);
		}
	}
};
}  // namespace kke
