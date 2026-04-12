#pragma once

#include <vector>

#include "kke/engine/d2d1/d2d1_headers.hh"

namespace kke {
template <typename T>
class PoolCache {
	using Ptr = Microsoft::WRL::ComPtr<T>;

	struct CachedPtr {
		bool isUsing;
		Ptr ptr;
	};

	std::vector<CachedPtr> storage;

public:
	void putAndUse(Ptr val) {
		storage.push_back({true, val});
	}

	Ptr tryUseCached() {
		for (auto& entry : storage) {
			if (entry.isUsing) {
				continue;
			}
			entry.isUsing = true;
			return entry.ptr;
		}
		return nullptr;
	}

	void release(Ptr val) {
		for (auto& entry : storage) {
			if (entry.ptr.Get() != val.Get()) {
				continue;
			}
			entry.isUsing = false;
		}
	}
};
}  // namespace kke
