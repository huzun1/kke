#include <Windows.h>

#include "Hooks.hh"

namespace kke::example::overlay {
class DllLifecycle {
  public:
	static void attach(HMODULE module) {
		DisableThreadLibraryCalls(module);
		HANDLE thread = CreateThread(nullptr, 0, initialize, nullptr, 0, nullptr);
		if (thread != nullptr) {
			CloseHandle(thread);
		}
	}

	static void detach() {
		Hooks::terminate();
	}

  private:
	static DWORD WINAPI initialize(LPVOID) {
		Hooks::initialize();
		return 0;
	}
};
} // namespace kke::example::overlay

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID) {
	if (reason == DLL_PROCESS_ATTACH) {
		kke::example::overlay::DllLifecycle::attach(module);
	} else if (reason == DLL_PROCESS_DETACH) {
		kke::example::overlay::DllLifecycle::detach();
	}
	return TRUE;
}
