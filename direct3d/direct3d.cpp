
#include "direct3d.hpp"

HMODULE module_handle() {
	static const HMODULE module = GetModuleHandle(nullptr);
	
	return module;
}

Rc<IDirect3D9> Direct3D() {
	SINGLETON(
		IDirect3D9,
		[]() { return Direct3DCreate9(D3D_SDK_VERSION); },
		[](LPDIRECT3D9 p) { p->Release(); }
	);
}

bool HResult::failed() const { return FAILED(result); }
bool HResult::success() const { return !failed(); }
void HResult::clear_result() { result = 0; }
HRESULT HResult::get_result() const { return result; }
