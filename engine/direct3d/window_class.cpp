
#include "window_class.hpp"

static LRESULT CALLBACK window_procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	auto payload = reinterpret_cast<WindowCallback *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	
	if (payload)
		return payload->window_callback(hWnd, message, wParam, lParam);
	else
		return DefWindowProc(hWnd, message, wParam, lParam);
}

WindowClass::WindowClass(LPCTSTR name) {
    ZeroMemory(&wc, sizeof(wc));
    
    //	Filling wndClass. You are to refer to MSDN for each of the members details.
    //	These are the fundamental structure members to be specify, in order to create your window.
    wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hInstance = module_handle();
    wc.lpfnWndProc = window_procedure;
    wc.lpszClassName = name;
    wc.style = CS_HREDRAW | CS_VREDRAW;

    //	Register the window.
    RegisterClass(&wc);
}

void WindowClass::reset() {
    if (class_name()) {
        UnregisterClass(class_name(), module_handle());
        wc.lpszClassName = nullptr;
    }
}
