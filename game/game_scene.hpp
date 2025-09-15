
#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "context.hpp"

// a visual effect (vfx)
struct Effect {
	Animation animation;
	Vec2f position;
	Channel channel;
	Transform transform {};
};

// a static wall
struct Wall: public virtual RectBody, public virtual MaskBody<> {
	Color color = Color(63, 63, 191);
	
	Wall(Vec2f pos, Vec2f size) {
		position = pos;
		this->size = size;
		layer = { WALL };
		is_static = true;
		friction = 0.5;
	}
};

// a trap that damages player
struct Grinder: public virtual CircleBody, public virtual MaskBody<> {
	Animation animation;
	float scale;
    Transform transform {};
	
    static Animation default_animation(Context &context) {
        static Box<Animation> anim;
        if (!anim) anim = new Animation {
            FPS / 15,
            8,
            [&context](int frame) { return context.sheets.shuriken.tile(frame); },
        };
        return *anim;
    }
    
	Grinder(Context &context, float scale, Vec2f pos): animation(default_animation(context)), scale(scale) {
		position = pos;
		radius = scale * 16;
        transform = Transform(context.sheets.shuriken.get_tile_size().to_f() / 2) + Scale(scale);
	}
};

// a bomb that player throws
struct Bomb: public virtual CircleBody, virtual MaskBody<> {
    static Animation explosion_animation(Context &context) {
        static Box<Animation> anim;
        if (!anim) anim = new Animation {
            FPS / 15,
            context.sheets.explosion.get_tile_count(),
            [&context](int frame) { return context.sheets.explosion.tile(frame); },
            false,
        };
        return *anim;
    }
    
	Bomb(Vec2f pos, Vec2f velocity) {
		position = pos;
		radius = 10;
		this->velocity = velocity;
		layer = { BOMB };
		mask = { WALL, BOMB };
		elasticity = 0.75;
	}
};

// an entity that has hp
struct Entity: public virtual RectBody, public virtual MaskBody<> {
	Box<Animation> animation;
	int direction = 0;
    int hp = 0;
	int invulnerable = 0;
    float moving_speed = 0;
    Transform transform {};
    Color modulate = Colors::WHITE;
	
	Entity(Vec2f pos, Vec2f size, int hp): hp(hp) {
		position = pos;
		this->size = size;
		mask = { WALL };
		elasticity = 0;
	}
    
	Entity(Vec2f pos, Vec2f size, int hp, Animation &&animation): Entity(pos, size, hp) {
        this->animation = new Animation(std::move(animation));
    }
    
    constexpr bool is_alive() const { return hp > 0; }
    constexpr bool is_dead() const { return !is_alive(); }
    constexpr bool is_invulnerable() const { return invulnerable > 0; }
    constexpr bool is_vulnerable() const { return !is_invulnerable(); }
    
    virtual void update(int frame) {
        if (is_invulnerable()) invulnerable -= frame;
        if (is_invulnerable()) modulate = invulnerable % 4 == 3 ? Color(127, 127, 127) : Colors::WHITE;
        velocity.x = static_cast<float>(direction) * moving_speed;
    }
    
    bool hurt(int amount = 1) {
        if (is_vulnerable()) {
            hp -= amount;
            invulnerable = 30;
            return true;
        }
        else return false;
    }
};

// the player
struct Player: public Entity {
    bool facing_right = true; // used in animation
	float jump_power = 7.5;
	bool double_jump = false;
    
    Player(Context &context): Entity({}, context.sheets.militia.get_tile_size().to_f(), 3, {
        FPS / 10,
        4,
        [&](int frame) {
			auto row = facing_right ? 2 : 1;
			auto col = on_floor ? frame : 1;
			
			return context.sheets.militia.tile(row, col);
		},
    }) {
        moving_speed = 10.f;
    }
    
    void update(int frame) override {
        Entity::update(frame);
        
        if (on_floor) double_jump = false;
        
        if (direction == 0 || !on_floor) animation->reset();
        else animation->update(frame);
    }
};

struct GameScene: public Scene<Context> {
	Label* hp_label = nullptr;
	StackContainer *stack = nullptr;
	
	RectBody camera {};
	RectBody large_camera {}; // used to deactivate bodies if they are far away from camera
	
	std::vector<Rc<Wall>> walls;
	std::vector<Rc<Grinder>> grinders;
	std::vector<Rc<Bomb>> bombs;
	std::vector<Effect> effects;
	
	Rc<Player> player;
	
	PhysicsEngine<> physics_engine {};
	
	TString hp_string;
    
	float noise_range = 500;
	
	bool paused = false;
	
	Channel music;
	
	void update_hp() {
		hp_string = format("HP Left: %d", player->hp);
		hp_label->set_text(hp_string);
		
		if (player->is_dead()) engine->scenes.pop_back();
	}
	
	Box<UI> pause_panel() {
		return new AlignContainer {
			{ Alignment::Center, Alignment::VCenter },
			new Panel {
				Color(127, 127, 255),
				0,
				Colors::ZERO,
				new Padding {
					10,
					new VContainer {
						new AlignContainer {
							Alignment::Center,
							new Text(TEXT("Paused"), {})
						},
						new HContainer {
							new Text(TEXT("Music"), {}),
							new Margin({ 100, 0 }),
							new Button {
								BUTTON_COLOR,
								BUTTON_BORDER_WIDTH,
								BUTTON_BORDER,
								{ 10, 10 },
								new Text(TEXT("-"), {}),
								[&]() {
									if (music) {
										auto volume = music.volume() - 0.1f;
										if (volume < 0) volume = 0;
										music.set_volume(volume);
									}
								}
							},
							new Margin({ 10, 0 }),
							new Button {
								BUTTON_COLOR,
								BUTTON_BORDER_WIDTH,
								BUTTON_BORDER,
								{ 10, 10 },
								new Text(TEXT("+"), {}),
								[&]() {
									if (music) {
										auto volume = music.volume() + 0.1f;
										if (volume > 1) volume = 1;
										music.set_volume(volume);
									}
								}
							},
						}
					}
				}
			}
		};
	}
	
	void init() override {
		ui = stack = new StackContainer {
			new Padding {
				20,
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
							new TextureUI { context->assets.pause },
							[&]() {
								paused = !paused;
								
								if (paused) {
									stack->children.push_back(pause_panel());
								}
								else {
									stack->children.pop_back();
								}
							},
						}
					},
				}
			}
		};
        
		camera.size = engine->resolution.to_f();
		large_camera.size = camera.size * 2;
		
        // register bodies
        
        physics_engine.bodies.push_back(player = std::make_shared<Player>(*context));
        
		for (auto &wall : walls) physics_engine.bodies.push_back(wall);
        
        update_hp();
	}
	
	void jump() {
		auto d = physics_engine.gravity_direction;
		
		if (d.x != 0) player->velocity.x = 0;
		if (d.y != 0) player->velocity.y = 0;
		
		player->velocity += -d * player->jump_power;
	}
	
	void physics() override {
		if (paused) return;
		
		if (engine->mouse) {
			if (engine->mouse.left_click() && !engine->click_consumed) { // throw bomb
				auto n = (engine->mouse_pos.to_f() - (player->position - camera.position)).normalize();
				auto bomb = std::make_shared<Bomb>(player->position, n * 10);
				
				bombs.push_back(bomb);
				physics_engine.bodies.push_back(bomb);
			}
            
			if (engine->mouse.right_click()) { // detonate bomb
				for (auto &bomb : bombs) {
					const auto &sounds = context->assets.explosion;
					const auto &sound = sounds[rand() % sounds.size()];
					
					Effect effect {
						Bomb::explosion_animation(*context),
						bomb->position,
						engine->fmod.channel(sound),
						Transform(context->sheets.explosion.get_tile_size().to_f() / 2) + Scale(Vec2(3, 3)),
					};
					
					effect.channel.play();
					
					effects.push_back(std::move(effect));
				}
				
				bombs.clear();
			}
		}
		
		if (engine->keyboard) {
			player->direction = 0;
			
			if (engine->keyboard.key_pressed(DIK_A)) player->direction--;
			if (engine->keyboard.key_pressed(DIK_D)) player->direction++;
			
			if (player->direction != 0) player->facing_right = player->direction == 1;
            
			if (engine->keyboard.key_just_pressed(DIK_W)) {
				if (player->on_floor) {
					jump();
					player->on_floor = false;
				}
				else if (!player->double_jump) {
					player->double_jump = true;
					jump();
				}
			}
		}
		
        player->update(engine->frame());
        
		physics_engine.process(engine->frame());
        
		// deactivate bodies that are too far away from camera
		
        for (auto &wall : walls) {
            wall->is_active = wall->is_colliding(large_camera);
        }
		
		for (auto& grinder : grinders) {
            grinder->is_active = grinder->is_colliding(large_camera);
            
			if (player->is_vulnerable() && player->is_colliding(*grinder)) {
                player->hurt();
                update_hp();
            }
		}
	}
	
	// adjust drawing by camera position
	Sprite& draw(const TextureRect &texture, Vec2f pos = Vec2(), Vec2i center = Vec2(), Color modulate = Colors::WHITE) {
		return engine->sprite.draw(texture, (pos - camera.position).to_i(), center, modulate);
	}
	
	// adjust drawing by camera position
	Sprite& draw(const TextureRect &texture, const Transform &transform, Color modulate = Colors::WHITE) {
		return engine->sprite.draw(texture, transform + Translate(-camera.position), modulate);
	}
	
    Sprite& draw_entity(const Entity &entity) {
        return draw(entity.animation->get_texture_rect(), entity.transform + Translate(entity.position), entity.modulate);
    }
    
	void render() override {
		camera.position = player->center() - CENTER;
		large_camera.position = player->center() - RESOLUTION;
		
		draw_entity(*player);
		
		for (auto& wall : walls) {
			if (!wall->is_active) continue;
			engine->sprite.draw_rect(wall->position - camera.position, wall->size, wall->color);
		}
		
		for (auto& grinder : grinders) {
			if (!grinder->is_active) continue;
			if (!paused) grinder->animation.update(engine->frame());
			draw(grinder->animation.get_texture_rect(), grinder->transform + Translate(grinder->position));
		}
        
		for (auto &bomb : bombs) {
			if (!bomb->is_colliding(large_camera)) continue;
			
			draw(context->sheets.bomb.tile(0), bomb->position, Vec2i(16, 24));
		}
		
		std::erase_if(effects, [&](auto& effect) {
			auto d = effect.position.x - camera.position.x;
			auto p = d / noise_range;
			
			if (effect.channel.update()) effect.channel.pan(std::clamp(p, -1.f, 1.f));
			
			if (effect.animation.playing())
				draw(effect.animation.get_texture_rect(), effect.transform + Translate(effect.position));
			
			effect.animation.update(engine->frame());
			
			// remove effect if animation and sound finished
			return effect.animation.finished() && !effect.channel;
		});
		
		music.update();
		
		if (!music) {
			music = engine->fmod.channel(context->assets.music);
			music.play();
		}
	}
	
	~GameScene() {
		if (music) music.stop();
	}
};

#endif
