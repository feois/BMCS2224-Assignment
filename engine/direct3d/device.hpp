
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
	
	// clear buffer and set background color
	void clear(Color color = Color()) const;
	
	// start rendering
	Device& begin() { result = device->BeginScene(); return *this; }
	// stop rendering
	Device& end() { result = device->EndScene(); return *this; }
	
	// present rendered scene
	void present() const;
	
	bool is_fullscreen() const { return !param.Windowed; }
	
	Device& set_fullscreen(bool fullscreen);
	
	WRAP(IDirect3DDevice9, device.get());
};

// a rendering resource
struct Renderer {
	virtual void begin() = 0; // start rendering
	virtual void end() = 0; // stop rendering
	virtual void lost() = 0; // prepare for device reset
	virtual void reset() = 0; // recover from device reset
	
	// flush the batched rendering
	void flush() {
		end();
		begin();
	}
};

#endif
