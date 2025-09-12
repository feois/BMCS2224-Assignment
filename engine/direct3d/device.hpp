
#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "direct3d.hpp"
#include "window.hpp"
#include <engine/core/color.hpp>
#include <engine/core/vec2.hpp>

class Texture;

class Device: public HResult {
	D3DPRESENT_PARAMETERS param;
	Box<IDirect3DDevice9, RELEASE(IDirect3DDevice9)> device;
	Vec2i size;
	Rc<IDirect3D9> direct3D;
	
public:
	Device(const Window &window, Vec2i size);
	
	void clear(Color color = Color()) const;
	
	Device& begin() { result = device->BeginScene(); return *this; }
	Device& end() { result = device->EndScene(); return *this; }
	
	void present() const;
	
	Vec2f scale(Vec2f) const;
	
	bool is_fullscreen() const { return !param.Windowed; }
	
	Device& set_fullscreen(bool fullscreen);
	
	WRAP(IDirect3DDevice9, device.get());
};

struct Renderer {
	virtual void begin() = 0;
	virtual void end() = 0;
	virtual void lost() = 0;
	virtual void reset() = 0;
	
	void flush() {
		end();
		begin();
	}
};

#endif
