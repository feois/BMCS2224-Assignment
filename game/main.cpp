
#include <engine/prelude.hpp>
#include "main_menu.hpp"

struct MyGame: public Engine<Context> {
    MyGame(int show): Engine(TITLE, Vec2(300, 300), RESOLUTION, FPS, show) {
		background = Colors::WHITE;
	}
    
    bool on_keydown(WPARAM key, [[maybe_unused]] LPARAM lParam) {
		switch (key) {
			case VK_ESCAPE:
				scenes.pop_back();
				if (scenes.empty()) quit();
				return true;
			
			case VK_F11:
				toggle_fullscreen();
				return true;
		}
		
		return false;
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int nShowCmd) {
    auto engine = std::make_unique<MyGame>(nShowCmd);
	auto context = std::make_unique<Context>(*engine);
    
	engine->pointer_texture = context->assets.pointer;
	engine->scenes.push_back(std::make_shared<MainMenu>());
    engine->run(*context);
	
	return 0;
}


