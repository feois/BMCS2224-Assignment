
#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "levels.hpp"

struct LevelSelect: public Scene<Context> {
	// a level button
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
						new TextureUI { root.context->sheets.shuriken.tile(0) },
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
	
	void init() override {
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

#endif
