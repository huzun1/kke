#pragma once

#include <vector>

#include "kke/engine/d2d/d2d1_headers.hh"

namespace kke {
template <typename T>
class PoolCache {
	struct CachedPtr {
		bool isUsing;
		T ptr;
	};

	std::vector<CachedPtr> storage;

public:
	void putAndUse(T val) {
		storage.push_back({true, val});
	}

	T& tryUse() {
		for (auto& entry : storage) {
			if (entry.isUsing) {
				continue;
			}
			entry.isUsing = true;
			return entry.ptr;
		}
		return nullptr;
	}

	void release(T val) {
		for (auto& entry : storage) {
			if (entry.ptr.Get() != val.Get()) {
				continue;
			}
			entry.isUsing = false;
		}
	}
};
}  // namespace kke
