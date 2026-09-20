#pragma once

namespace kke::benchmark {
class ShadowCacheControl {
	inline static bool isCacheEnabled = true;

  public:
	static bool isEnabled() {
		return isCacheEnabled;
	}

	static void setEnabled(bool value) {
		isCacheEnabled = value;
	}
};
} // namespace kke::benchmark
