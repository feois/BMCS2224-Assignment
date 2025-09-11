
#include "dependencies.hpp"
#include "utils.hpp"

HMODULE module_handle() {
	static const HMODULE module = GetModuleHandle(nullptr);
	
	return module;
}

std::shared_ptr<IDirect3D9> Direct3D() {
	SINGLETON(
		IDirect3D9,
		[]() { return Direct3DCreate9(D3D_SDK_VERSION); },
		[](LPDIRECT3D9 p) { p->Release(); }
	);
}
