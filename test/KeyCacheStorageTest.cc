#include <cstdint>

#include "kke/engine/d2d/resource/KeyCacheStorage.hh"

class CacheValue final : public IUnknown {
	ULONG referenceCount = 1;

  public:
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID interfaceId, void** object) override {
		if (object == nullptr) {
			return E_POINTER;
		}
		if (interfaceId != IID_IUnknown) {
			*object = nullptr;
			return E_NOINTERFACE;
		}
		*object = static_cast<IUnknown*>(this);
		AddRef();
		return S_OK;
	}

	ULONG STDMETHODCALLTYPE AddRef() override {
		return ++referenceCount;
	}

	ULONG STDMETHODCALLTYPE Release() override {
		ULONG const remainingReferences = --referenceCount;
		if (remainingReferences == 0) {
			delete this;
		}
		return remainingReferences;
	}
};

class KeyCacheStorageTest {
	using CacheKey = std::uint64_t;
	using Cache = kke::KeyCacheStorage<IUnknown>;
	using Value = Microsoft::WRL::ComPtr<IUnknown>;

  public:
	static bool run() {
		return evictsOldestUnusedEntry() && rejectsUnusedEntryWhenCacheIsHot() && clearsEntries() &&
			   replacesExistingEntry() && supportsDisabledCache();
	}

  private:
	static bool evictsOldestUnusedEntry() {
		Cache cache(8);
		Value values[9];
		for (CacheKey key = 0; key < 8; ++key) {
			values[key].Attach(new CacheValue());
			cache.put(key, values[key]);
		}
		for (CacheKey key = 0; key < 6; ++key) {
			for (CacheKey access = key; access < 6; ++access) {
				if (cache.get(key) != values[key]) {
					return false;
				}
			}
		}

		values[8].Attach(new CacheValue());
		cache.put(8, values[8]);
		for (CacheKey key = 0; key < 6; ++key) {
			if (cache.get(key) != values[key]) {
				return false;
			}
		}
		return !cache.get(6) && cache.get(7) == values[7] && cache.get(8) == values[8];
	}

	static bool rejectsUnusedEntryWhenCacheIsHot() {
		Cache cache(2);
		Value values[3];
		for (CacheKey key = 0; key < 2; ++key) {
			values[key].Attach(new CacheValue());
			cache.put(key, values[key]);
			if (cache.get(key) != values[key]) {
				return false;
			}
		}

		values[2].Attach(new CacheValue());
		cache.put(2, values[2]);
		return cache.get(0) == values[0] && cache.get(1) == values[1] && !cache.get(2);
	}

	static bool clearsEntries() {
		Cache cache(2);
		Value value;
		value.Attach(new CacheValue());
		cache.put(1, value);
		cache.clear();
		return !cache.get(1);
	}

	static bool replacesExistingEntry() {
		Cache cache(1);
		Value first;
		Value second;
		first.Attach(new CacheValue());
		second.Attach(new CacheValue());

		cache.put(1, first);
		cache.put(1, second);
		if (cache.get(1) != second) {
			return false;
		}
		cache.put(1, first);
		return cache.get(1) == first;
	}

	static bool supportsDisabledCache() {
		Cache cache(0);
		Value value;
		value.Attach(new CacheValue());
		cache.put(1, value);
		return !cache.get(1);
	}
};

int main() {
	return KeyCacheStorageTest::run() ? 0 : 1;
}
