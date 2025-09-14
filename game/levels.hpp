
#ifndef GAME_LEVELS_HPP
#define GAME_LEVELS_HPP

#include "game_scene.hpp"

struct Level1: public GameScene {
    void init() override {
		walls = {
			std::make_shared<Wall>(Wall { Vec2(0, 500), Vec2(1000, 1000) }),
			std::make_shared<Wall>(Wall { Vec2(500, 400), Vec2(1000, 1000) }),
			std::make_shared<Wall>(Wall { Vec2(1000, 200), Vec2(1000, 1000) }),
		};
        
        grinders = {
			std::make_shared<Grinder>(Grinder { *context, 3.f, Vec2(200, 480) }),
		};
        
        GameScene::init();
    }
};

struct Level2: public GameScene {
	void init() override {
		walls = {
			std::make_shared<Wall>(Wall { Vec2(-1000, 500), Vec2(2000, 500) }),
			std::make_shared<Wall>(Wall { Vec2(-2000, 0), Vec2(1000, 1000) }),
			std::make_shared<Wall>(Wall { Vec2(+1000, 0), Vec2(1000, 1000) }),
		};
        
        GameScene::init();
	}
};

struct Level3: public GameScene {
	void init() override {
		GameScene::init();
	}
};

#endif
