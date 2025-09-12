
#ifndef DEPENDENCIES_HPP
#define DEPENDENCIES_HPP

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <engine/core/utils.hpp>
#include <engine/core/result.hpp>

HMODULE module_handle();

Rc<IDirect3D9> Direct3D();

class HResult: public Result {
protected:
	HRESULT result = 0;
	
public:
	bool failed() const override;
	bool success() const override;
	void clear_result() override;
	HRESULT get_result() const;
};

#endif
