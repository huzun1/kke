#pragma once

#include <cassert>
#include <cstdint>
#include <list>
#include <unordered_map>
#include <utility>

#include "kke/engine/d2d/d2d1_headers.hh"

namespace kke {
template <typename T> class KeyCacheStorage {
	using CacheKey = uint64_t;
	using Ptr = Microsoft::WRL::ComPtr<T>;
	using UnusedKeyList = std::list<CacheKey>;

	struct CachedPtr {
		Ptr ptr;
		UnusedKeyList::iterator unusedKey;
		bool hasBeenUsed;
	};

	uint32_t limit;
	std::unordered_map<CacheKey, CachedPtr> storage;
	UnusedKeyList unusedKeys;

  public:
	KeyCacheStorage(uint32_t limit = UINT32_MAX) : limit(limit) {
	}

	/**
	 * @brief Store instances in the cache. When the limit is exceeded, remove the oldest entry
	 * that has not been retrieved.
	 */
	void put(CacheKey key, Ptr val) {
		auto existing = storage.find(key);
		if (existing != storage.end()) {
			if (!existing->second.hasBeenUsed) {
				unusedKeys.erase(existing->second.unusedKey);
			}
			storage.erase(existing);
		}

		unusedKeys.push_back(key);
		auto unusedKey = unusedKeys.end();
		--unusedKey;
		storage.emplace(key, CachedPtr{std::move(val), unusedKey, false});
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
		if (!it->second.hasBeenUsed) {
			unusedKeys.erase(it->second.unusedKey);
			it->second.hasBeenUsed = true;
		}
		return it->second.ptr;
	}

	void clear() {
		storage.clear();
		unusedKeys.clear();
	}

  private:
	void clean() {
		while (storage.size() > limit) {
			assert(!unusedKeys.empty());
			CacheKey const key = unusedKeys.front();
			unusedKeys.pop_front();
			storage.erase(key);
		}
	}
};
} // namespace kke
