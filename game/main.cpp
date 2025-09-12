
#include <engine/engine.hpp>
#include <physics/physics.hpp>
#include <ui/prelude.hpp>

constexpr auto RESOLUTION = Vec2(800, 600);
constexpr auto CENTER = Vec2(RESOLUTION.i() / 2);
constexpr int FPS = 60;
constexpr int LEVELS = 3;
constexpr static LPCTSTR TITLE = TEXT("Platformer");
constexpr auto BUTTON_COLOR = Color(127, 127, 191);
constexpr auto BUTTON_BORDER = Color(63, 63, 127);
constexpr auto BUTTON_BORDER_WIDTH = 1;

#define ASSETS_DIR "assets/"

struct Assets {
	std::shared_ptr<Texture> pointer;
	Texture militia, shuriken, bomb, explosion_effect, pause;
	std::array<Sound, 6> explosion;
};

struct Sheets {
	SpriteSheet militia, shuriken, bomb, explosion;
};

struct Context {
	Assets assets;
	Sheets sheets;
	int level = 0;
	
	Context(Engine<Context> &engine)
	: assets {
		.pointer = std::make_unique<Texture>(engine.device, TEXT(ASSETS_DIR "pointer.png")),
		.militia = Texture(engine.device, TEXT(ASSETS_DIR "militia.png")),
		.shuriken = Texture(engine.device, TEXT(ASSETS_DIR "Shuriken.png")),
		.bomb = Texture(engine.device, TEXT(ASSETS_DIR "Bomb.png")),
		.explosion_effect = Texture(engine.device, TEXT(ASSETS_DIR "Blast.png")),
		.pause = Texture(engine.device, TEXT(ASSETS_DIR "pause.png")),
		.explosion = {
			Sound(engine.fmod, ASSETS_DIR "explosion1.ogg"),
			Sound(engine.fmod, ASSETS_DIR "explosion2.ogg"),
			Sound(engine.fmod, ASSETS_DIR "explosion3.ogg"),
			Sound(engine.fmod, ASSETS_DIR "explosion4.ogg"),
			Sound(engine.fmod, ASSETS_DIR "explosion5.ogg"),
			Sound(engine.fmod, ASSETS_DIR "explosion6.ogg"),
		},
	}, sheets {
		.militia = SpriteSheet::with_sheet_size(assets.militia, Vec2(4, 4)),
		.shuriken = SpriteSheet::with_sheet_size(assets.shuriken, Vec2(8, 1)),
		.bomb = SpriteSheet::with_sheet_size(assets.bomb, Vec2(5, 1)),
		.explosion = SpriteSheet::with_sheet_size(assets.explosion_effect, Vec2(3, 3)),
	} {}
};

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

struct Wall: public Rect {
	Color color = Color(63, 63, 191);
	
	Wall(Vec2f pos, Vec2f size): Rect(pos, size) {}
};

struct Grinder: public Circle {
	Animation animation;
	float scale;
	
	Grinder(float scale, Vec2f pos, Animation anim): Circle(pos, scale * 16), animation(anim), scale(scale) {}
};

struct Entity: public Rect {
	bool on_floor = false;
	Box<Animation> animation;
	Direction direction = Direction::None;
	
	Entity(Vec2f pos, Vec2f size): Rect(pos, size) {}
};

struct GameScene: public Scene<Context> {
	Label* hp_label = nullptr;
	TextureUI* pause_texture = nullptr;
	
	Rect camera { -CENTER.f(), RESOLUTION };
	Rect large_camera { -RESOLUTION.f(), RESOLUTION.f() * 2 };
	
	std::vector<std::shared_ptr<Wall>> walls;
	std::vector<std::shared_ptr<Grinder>> grinders;
	
	Entity player { Vec2(), Vec2() };
	Direction gravity_direction = Direction::Down;
	float gravity_speed = 9.8f / FPS;
	
	Direction player_last_direction = Direction::Right;
	
	std::vector<std::shared_ptr<Wall>> active_walls;
	std::vector<std::shared_ptr<Grinder>> active_grinders;
	
	int hp = 3;
	TString hp_string;
	int invulnerable = 0;
	float jump_power = 7.5;
	bool double_jump = false;
	
	bool paused = false;
	
	Animation grinder_animation {
		FPS / 15,
		8,
		[&](int frame) { return context->sheets.shuriken.tile(frame); },
	};
	
	GameScene() {
		ui = new Padding {
			{ 20, 20 },
			new StackContainer {
				new AlignContainer {
					{ Alignment::Top, Alignment::Right },
					hp_label = new Label {
						TEXT(""),
						Colors::ZERO,
						0,
						Colors::ZERO,
						{},
						{}
					}
				},
				new AlignContainer {
					{ Alignment::Top, Alignment::Left },
					new Button {
						BUTTON_COLOR,
						BUTTON_BORDER_WIDTH,
						BUTTON_BORDER,
						{},
						pause_texture = new TextureUI { {} },
						[&]() { paused = !paused; },
					}
				},
			}
		};
		
		player.animation = new Animation(FPS / 12, 4, [&](int frame) {
			auto row = player_last_direction == Direction::Left ? 1 : 2;
			auto col = player.on_floor ? frame : 1;
			
			return context->sheets.militia.tile(row, col);
		});
		
		update_hp();
	}
	
	Vec2f gravity() const { return Vec2i(gravity_direction).to_f() * gravity_speed; }
	
	void update_hp() {
		hp_string = format("HP Left: %d", hp);
		hp_label->set_text(hp_string);
		
		if (hp <= 0) engine->scenes.pop_back();
	}
	
	void init() override {
		player.size = context->sheets.militia.get_tile_size().to_f();
		pause_texture->texture = context->assets.pause;
	}
	
	void check_active() {
		active_walls.clear();
		active_grinders.clear();
		
		for (auto& wall : walls) {
			if (wall->check_collision(&large_camera)) active_walls.push_back(wall);
		}
		
		for (auto& grinder : grinders) {
			if (grinder->check_collision(&large_camera)) active_grinders.push_back(grinder);
		}
	}
	
	void entity_physics(Entity &entity) {
		if (!entity.on_floor) entity.velocity += gravity() * static_cast<float>(engine->frame());
		
		entity.position += entity.velocity * static_cast<float>(engine->frame());
		entity.on_floor = false;
	}
	
	void entity_wall(Entity &entity, Wall &wall) {
		if (entity.check_collision(&wall)) {
			auto p = Vec2i(wall.push_out(entity));
			auto gd = -Vec2i(gravity_direction);
			
			if ((p.x == gd.x && p.x != 0) || (p.y == gd.y && p.y != 0)) entity.on_floor = true;
		}
	}
	
	void jump() {
		auto d = Vec2i(gravity_direction);
		
		if (d.x != 0) player.velocity.x = 0;
		if (d.y != 0) player.velocity.y = 0;
		
		player.velocity += -d.to_f() * jump_power;
	}
	
	void physics() override {
		if (paused) return;
		
		if (player.on_floor) {
			double_jump = false;
		}
		
		if (engine->keyboard) {
			auto d = Direction::None;
			
			if (engine->keyboard.key_pressed(DIK_A)) d += Direction::Left;
			if (engine->keyboard.key_pressed(DIK_D)) d += Direction::Right;
			
			player.direction = d;
			
			if (d != Direction::None) player_last_direction = player.direction;
			
			player.velocity.x = static_cast<float>(Vec2i(d).x * 10);
			
			if (engine->keyboard.key_just_pressed(DIK_W)) {
				if (player.on_floor) {
					jump();
					player.on_floor = false;
				}
				else if (!double_jump) {
					double_jump = true;
					jump();
				}
			}
		}
		
		check_active();
		
		entity_physics(player);
		
		for (auto& wall : active_walls) {
			entity_wall(player, *wall);
		}
		
		for (auto& grinder : active_grinders) {
			if ((!invulnerable) && player.check_collision(&*grinder)) {
				hp -= 1;
				update_hp();
				invulnerable = 30;
			}
		}
		
		if (invulnerable > 0) invulnerable -= engine->frame();
		if (invulnerable < 0) invulnerable = 0;
	}
	
	Sprite& draw(const TextureRect &texture, Vec2f pos = Vec2(), Vec2i center = Vec2(), Color modulate = Colors::WHITE) {
		return engine->sprite.draw(texture, (pos - camera.position).to_i(), center, modulate);
	}
	
	Sprite& draw(const TextureRect &texture, const Transform &transform, Color modulate = Colors::WHITE) {
		return engine->sprite.draw(texture, transform + Translate(-camera.position), modulate);
	}
	
	void render() override {
		camera.position = player.center() - CENTER;
		large_camera.position = player.center() - RESOLUTION;
		
		if (!paused) {
			if (player.direction == Direction::None || !player.on_floor) player.animation->reset();
			else player.animation->update(engine->frame());
		}

		draw(
			player.animation->get_texture_rect(),
			player.position,
			Vec2(),
			invulnerable % 3 == 1 ? Color(127, 127, 127) : Colors::WHITE
		);
		
		for (auto& wall : active_walls) {
			engine->sprite.draw_rect(wall->position - camera.position, wall->size, wall->color);
		}
		
		for (auto& grinder : active_grinders) {
			auto transform = Transform(context->sheets.shuriken.get_tile_size().to_f() / 2)
				+ Scale(grinder->scale)
				+ Translate(grinder->position);
			
			if (!paused) grinder->animation.update(engine->frame());
			draw(grinder->animation.get_texture_rect(), transform);
		}
	}
};

struct Level1: public GameScene {
	Level1(): GameScene() {
		walls = {
			std::make_shared<Wall>(Wall { Vec2(0, 500), Vec2(1000, 1000) }),
			std::make_shared<Wall>(Wall { Vec2(500, 400), Vec2(1000, 1000) }),
			std::make_shared<Wall>(Wall { Vec2(1000, 200), Vec2(1000, 1000) }),
		};
		
		grinders = {
			std::make_shared<Grinder>(Grinder { 3.f, Vec2(200, 480), grinder_animation }),
		};
	}
};

struct Bomb: public Circle {
	float mass;
	
	Bomb(Vec2f pos, float mass, Vec2f velocity): Circle(pos, 10, velocity), mass(mass) {}
};

struct Effect {
	Animation animation;
	Vec2f position;
	Channel channel;
	Transform transform {};
};

struct Level2: public GameScene {
	std::vector<Bomb> bombs;
	std::vector<Effect> effects;
	
	Animation explosion_animation {
		FPS / 15,
		6,
		[&](int frame) { return context->sheets.explosion.tile(frame); },
		false,
	};
	float noise_range = 500;
	float friction = 0.9f;
	
	Level2(): GameScene() {
		walls = {
			std::make_shared<Wall>(Wall { Vec2(-1000, 500), Vec2(2000, 500) }),
			std::make_shared<Wall>(Wall { Vec2(-2000, 0), Vec2(1000, 1000) }),
			std::make_shared<Wall>(Wall { Vec2(+1000, 0), Vec2(1000, 1000) }),
		};
	}
	
	void physics() override {
		GameScene::physics();
		
		if (engine->mouse) {
			if (engine->mouse.right_click()) {
				auto n = (engine->mouse_pos.to_f() - (player.position - camera.position)).normalize();
				
				bombs.push_back(Bomb(player.position, 10, n * 10));
			}
		}
		
		if (engine->keyboard) {
			if (engine->keyboard.key_just_pressed(DIK_SPACE)) {
				for (Bomb &bomb : bombs) {
					const auto &sounds = context->assets.explosion;
					const auto &sound = sounds[rand() % sounds.size()];
					
					Effect effect {
						explosion_animation,
						bomb.position,
						engine->fmod.channel(sound),
						Transform(context->sheets.explosion.get_tile_size().to_f() / 2) + Scale(Vec2(3, 3)),
					};
					
					effect.channel.play();
					
					effects.push_back(std::move(effect));
				}
				
				bombs.clear();
			}
		}
		
		for (size_t i = 0; i < bombs.size(); i++) {
			Bomb &bomb = bombs[i];
			bool g = true;
			
			for (auto &wall : active_walls) {
				if (wall->check_collision(&bomb)) {
					bomb.reflect(*wall, 0.75);
					g = false;
				}
			}
			
			if (bomb.check_collision(&player)) {
				bomb.reflect(player, 0.75);
			}
			
			for (size_t j = i + 1; j < bombs.size(); j++) {
				if (bomb.check_collision(&bombs[j])) {
					bomb.collide(bomb.mass, bombs[j], bombs[j].mass);
				}
			}
			
			if (g) bomb.velocity += gravity() * static_cast<float>(engine->frame());
			else bomb.velocity *= static_cast<float>(std::pow(friction, engine->frame()));
			
			bomb.position += bomb.velocity * static_cast<float>(engine->frame());
		}
	}
	
	void render() {
		GameScene::render();
		
		for (Bomb &bomb : bombs) draw(context->sheets.bomb.tile(0), bomb.position, Vec2i(16, 24));
		
		effects.erase(
			std::remove_if(effects.begin(), effects.end(), [&](auto& effect) {
				auto d = effect.position.x - player.position.x;
				auto p = d / noise_range;
				
				if (effect.channel.update()) effect.channel.pan(std::clamp(p, -1.f, 1.f));
				
				if (effect.animation.playing())
					draw(effect.animation.get_texture_rect(), effect.transform + Translate(effect.position));
				
				effect.animation.update(engine->frame());
				
				// remove effect if animation and sound finished
				return effect.animation.finished() && !effect.channel;
			}),
			effects.end()
		);
	}
};

struct Level3: public GameScene {
	Level3(): GameScene() {
		
	}
};

struct LevelSelect: public Scene<Context> {
	struct Level: public CompositeUI<Button> {
		constexpr static auto SIZE = Vec2(150, 150);
		constexpr static auto COLOR = Color(127, 191, 127);
		
		int level;
		TString title;
		LevelSelect *root;
		
		Text *text;
		
		Level(LevelSelect &root, int level)
		: CompositeUI {
			Button {
				COLOR,
				0,
				Colors::ZERO,
				{ 10, 10 },
				new VContainer {
					Stretch(1, new AlignContainer {
						{ Alignment::Center, Alignment::VCenter },
						new Margin({}) // todo dummy
					}),
					new AlignContainer {
						Alignment::Center,
						text = new Text {
							TEXT(""),
							{},
						},
					},
				},
				[&]() {
					Scene *scene = nullptr;
					
					switch (this->level) {
						case 1: scene = new Level1(); break;
						case 2: scene = new Level2(); break;
						case 3: scene = new Level3(); break;
					}
					
					if (scene) this->root->engine->scenes.push_back(Rc<Scene>(scene));
				}
			}
		}, level(level), root(&root) {
			title = format("Level %d", level);
			
			text->text = title;
		}
		
		void calc_min_size(Drawer &drawer) { ui.calc_min_size(drawer); min_size = SIZE; }
		bool on_click() override { return ui.on_click(); }
	};
	
	LevelSelect() {
		ui = new Padding {
			{ 100, 100 },
			new AlignContainer {
				Alignment::VCenter,
				new VContainer {
					new HContainer {
						new Level(*this, 1),
						Stretch { 1, new Margin({}) },
						new Level(*this, 2),
						Stretch { 1, new Margin({}) },
						new Level(*this, 3),
					},
					new Margin({ 0, 100 }),
					new AlignContainer {
						Alignment::Center,
						new Button {
							BUTTON_COLOR,
							BUTTON_BORDER_WIDTH,
							BUTTON_BORDER,
							{ 10, 10 },
							new Text { TEXT("Return to main menu"), {} },
							[&]() { engine->scenes.pop_back(); }
						},
					},
				}
			}
		};
	}
};

struct MainMenu: public Scene<Context> {
	MainMenu() {
		ui = new AlignContainer {
			{ Alignment::Center, Alignment::VCenter },
			new VContainer {
				new Label {
					TITLE,
					Colors::ZERO,
					0,
					Colors::ZERO,
					{ 10, 10 },
					{}
				},
				new Margin({ 0, 100 }),
				new AlignContainer {
					Alignment::Center,
					new Button {
						BUTTON_COLOR,
						BUTTON_BORDER_WIDTH,
						BUTTON_BORDER,
						{ 10, 10 },
						new Text { TEXT("Play"), {} },
						[&]() { engine->scenes.push_back(std::make_shared<LevelSelect>()); },
					},
				},
				new Margin({ 0, 50 }),
				new AlignContainer {
					Alignment::Center,
					new Button {
						BUTTON_COLOR,
						BUTTON_BORDER_WIDTH,
						BUTTON_BORDER,
						{ 10, 10 },
						new Text { TEXT("Exit"), {} },
						[&]() { engine->quit(); },
					},
				},
			},
		};
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


