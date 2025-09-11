
#ifndef DEPENDENCIES_HPP
#define DEPENDENCIES_HPP

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <memory>

HMODULE module_handle();

std::shared_ptr<IDirect3D9> Direct3D();

#endif
