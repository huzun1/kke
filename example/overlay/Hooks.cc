#include "Hooks.hh"

#include <MinHook.h>

#include "third_party/kiero/Kiero.hh"

namespace kke::example::overlay {
bool Hooks::initialize() {
	isStopping.store(false, std::memory_order_release);

	Kiero::Status::Enum kieroStatus = Kiero::Status::UnknownError;
	for (uint32_t attempt = 0; attempt < 100; ++attempt) {
		kieroStatus = Kiero::initialize(Kiero::RenderType::D3D11);
		if (kieroStatus == Kiero::Status::Success) {
			break;
		}
		Sleep(50);
	}
	if (kieroStatus != Kiero::Status::Success) {
		return false;
	}

	MH_STATUS minHookStatus = MH_Initialize();
	if (minHookStatus != MH_OK && minHookStatus != MH_ERROR_ALREADY_INITIALIZED) {
		Kiero::shutdown();
		return false;
	}
	ownsMinHook = minHookStatus == MH_OK;

	if (!installHooks()) {
		removeHooks();
		if (ownsMinHook) {
			MH_Uninitialize();
		}
		ownsMinHook = false;
		Kiero::shutdown();
		return false;
	}

	d2dEntrance = std::make_unique<D2dEntrance>();
	return true;
}

bool Hooks::terminate() {
	isStopping.store(true, std::memory_order_release);
	if (presentTarget != nullptr) {
		MH_DisableHook(presentTarget);
	}
	if (resizeBuffersTarget != nullptr) {
		MH_DisableHook(resizeBuffersTarget);
	}
	if (!drainCallbacks()) {
		return false;
	}

	if (d2dEntrance) {
		d2dEntrance->shutdown();
		d2dEntrance.reset();
	}
	removeHooks();
	if (ownsMinHook) {
		MH_Uninitialize();
	}
	ownsMinHook = false;
	Kiero::shutdown();
	originalPresent = nullptr;
	originalResizeBuffers = nullptr;
	return true;
}

HRESULT STDMETHODCALLTYPE
Hooks::onPresent(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) {
	callbackCount.fetch_add(1, std::memory_order_acq_rel);
	if (!isStopping.load(std::memory_order_acquire) && d2dEntrance) {
		d2dEntrance->renderFrame(swapChain);
	}

	HRESULT result = originalPresent(swapChain, syncInterval, flags);
	callbackCount.fetch_sub(1, std::memory_order_acq_rel);
	return result;
}

HRESULT STDMETHODCALLTYPE Hooks::onResizeBuffers(
	IDXGISwapChain* swapChain,
	UINT bufferCount,
	UINT width,
	UINT height,
	DXGI_FORMAT format,
	UINT flags
) {
	callbackCount.fetch_add(1, std::memory_order_acq_rel);
	if (!isStopping.load(std::memory_order_acquire) && d2dEntrance) {
		d2dEntrance->releaseBackBuffer();
	}

	HRESULT result = originalResizeBuffers(swapChain, bufferCount, width, height, format, flags);
	callbackCount.fetch_sub(1, std::memory_order_acq_rel);
	return result;
}

bool Hooks::installHooks() {
	uint150_t* methods = Kiero::getMethodsTable();
	if (methods == nullptr) {
		return false;
	}

	presentTarget = reinterpret_cast<void*>(methods[presentSlot]);
	resizeBuffersTarget = reinterpret_cast<void*>(methods[resizeBuffersSlot]);
	if (MH_CreateHook(
			presentTarget,
			reinterpret_cast<void*>(&onPresent),
			reinterpret_cast<void**>(&originalPresent)
		) != MH_OK) {
		return false;
	}
	if (MH_CreateHook(
			resizeBuffersTarget,
			reinterpret_cast<void*>(&onResizeBuffers),
			reinterpret_cast<void**>(&originalResizeBuffers)
		) != MH_OK) {
		return false;
	}

	if (MH_EnableHook(presentTarget) != MH_OK) {
		return false;
	}
	return MH_EnableHook(resizeBuffersTarget) == MH_OK;
}

void Hooks::removeHooks() {
	if (presentTarget != nullptr) {
		MH_DisableHook(presentTarget);
		MH_RemoveHook(presentTarget);
		presentTarget = nullptr;
	}
	if (resizeBuffersTarget != nullptr) {
		MH_DisableHook(resizeBuffersTarget);
		MH_RemoveHook(resizeBuffersTarget);
		resizeBuffersTarget = nullptr;
	}
}

bool Hooks::drainCallbacks() {
	ULONGLONG deadline = GetTickCount64() + 5000;
	while (callbackCount.load(std::memory_order_acquire) != 0 && GetTickCount64() < deadline) {
		Sleep(1);
	}
	return callbackCount.load(std::memory_order_acquire) == 0;
}
} // namespace kke::example::overlay
