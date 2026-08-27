#pragma once

#include <atomic>
#include <cstdint>
#include <memory>

#include <Windows.h>
#include <dxgi.h>

#include "D2dEntrance.hh"

namespace kke::example::overlay {
class Hooks {
	static constexpr uint32_t presentSlot = 8;
	static constexpr uint32_t resizeBuffersSlot = 13;

	using Present = HRESULT(STDMETHODCALLTYPE*)(IDXGISwapChain*, UINT, UINT);
	using ResizeBuffers =
		HRESULT(STDMETHODCALLTYPE*)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);

	static inline Present originalPresent = nullptr;
	static inline ResizeBuffers originalResizeBuffers = nullptr;
	static inline void* presentTarget = nullptr;
	static inline void* resizeBuffersTarget = nullptr;
	static inline std::unique_ptr<D2dEntrance> d2dEntrance;
	static inline std::atomic_uint32_t callbackCount = 0;
	static inline std::atomic_bool isStopping = false;
	static inline bool ownsMinHook = false;

  public:
	static bool initialize();

	static bool terminate();

  private:
	static HRESULT STDMETHODCALLTYPE
	onPresent(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags);

	static HRESULT STDMETHODCALLTYPE onResizeBuffers(
		IDXGISwapChain* swapChain,
		UINT bufferCount,
		UINT width,
		UINT height,
		DXGI_FORMAT format,
		UINT flags
	);

	static bool installHooks();

	static void removeHooks();

	static bool drainCallbacks();
};
} // namespace kke::example::overlay
