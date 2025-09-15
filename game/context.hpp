
#ifndef GAME_CONTEXT_HPP
#define GAME_CONTEXT_HPP

#include <engine/prelude.hpp>

inline constexpr auto RESOLUTION = Vec2(800, 600);
inline constexpr auto CENTER = Vec2(RESOLUTION.i() / 2);
inline constexpr int FPS = 60;
inline constexpr int LEVELS = 3;
inline constexpr LPCTSTR TITLE = TEXT("Platformer");
inline constexpr auto BUTTON_COLOR = Color(127, 127, 191);
inline constexpr auto BUTTON_BORDER = Color(63, 63, 127);
inline constexpr auto BUTTON_BORDER_WIDTH = 1;

enum PhysicsMask: unsigned char {
	PLAYER,
	WALL,
	BOMB,
	GRINDER,
};

#define ASSETS_DIR "game/assets/"

// contains all asset resources
struct Assets {
	Rc<Texture> pointer;
	Texture militia, shuriken, bomb, explosion_effect, pause;
	std::array<Sound, 6> explosion;
	Sound music;
};

// contains all sprite sheets
struct Sheets {
	SpriteSheet militia, shuriken, bomb, explosion;
};

// context type, holds all resources used in game
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
		.explosion_effect = Texture(engine.device, TEXT(ASSETS_DIR "explosion.png")),
		.pause = Texture(engine.device, TEXT(ASSETS_DIR "pause.png")),
		.explosion = {
			Sound(engine.fmod, ASSETS_DIR "explosion1.ogg"),
			Sound(engine.fmod, ASSETS_DIR "explosion2.ogg"),
			Sound(engine.fmod, ASSETS_DIR "explosion3.ogg"),
			Sound(engine.fmod, ASSETS_DIR "explosion4.ogg"),
			Sound(engine.fmod, ASSETS_DIR "explosion5.ogg"),
			Sound(engine.fmod, ASSETS_DIR "explosion6.ogg"),
		},
		.music = Sound(engine.fmod, ASSETS_DIR "Dusk.mp3"),
	}, sheets {
		.militia = SpriteSheet::with_sheet_size(assets.militia, Vec2(4, 4)),
		.shuriken = SpriteSheet::with_sheet_size(assets.shuriken, Vec2(8, 1)),
		.bomb = SpriteSheet::with_sheet_size(assets.bomb, Vec2(5, 1)),
		.explosion = SpriteSheet::with_tile_size(assets.explosion_effect, Vec2(32, 32), 11),
	} {}
};

#endif
