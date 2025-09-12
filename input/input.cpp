
#include "input.hpp"

Vec2i get_mouse_pos() {
	POINT point;
	GetCursorPos(&point);
	return Vec2i(point.x, point.y);
}

std::shared_ptr<IDirectInput8> DirectInput() {
	SINGLETON(
		IDirectInput8,
		[]() {
			LPVOID p = nullptr;
			
			return SUCCEEDED(DirectInput8Create(module_handle(), DIRECTINPUT_VERSION, IID_IDirectInput8, &p, nullptr))
				? static_cast<LPDIRECTINPUT8>(p)
				: nullptr;
		},
		[](LPDIRECTINPUT8 p) { p->Release(); }
	);
}

RawInputDevice::RawInputDevice(REFGUID guid, LPCDIDATAFORMAT format) {
	if ((direct_input = DirectInput())) {
		LPDIRECTINPUTDEVICE8 p;
		result = direct_input->CreateDevice(guid, &p, nullptr);
		device = p;
		if (success()) result = device->SetDataFormat(format);
		if (failed()) device = nullptr;
	}
}

bool RawInputDevice::acquire(const Window &window, bool background, bool exclusive) {
	if (acquired) return false;
	if (!device) return false;
	result = device->SetCooperativeLevel(
		window.get_handle(),
		(background ? DISCL_BACKGROUND : DISCL_FOREGROUND) | (exclusive ? DISCL_EXCLUSIVE : DISCL_NONEXCLUSIVE)
	);
	if (success()) result = device->Acquire();
	return acquired = success();
}

void RawInputDevice::unacquire() {
	if (acquired) result = device->Unacquire();
	acquired = false;
}
