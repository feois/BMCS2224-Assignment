
#ifndef GAME_HPP
#define GAME_HPP

#include <engine/direct3d/font.hpp>
#include <engine/input/keyboard.hpp>
#include <engine/input/mouse.hpp>
#include <engine/audio/audio.hpp>
#include <engine/ui/ui.hpp>
#include "sprite_sheet.hpp"
#include "timer.hpp"
#include "animation.hpp"
#include "utils.hpp"
#include <algorithm>

template<typename C>
class Engine;

template<typename C>
class Scene {
protected:
    Engine<C> *engine = nullptr;
    C* context = nullptr;
    Box<UI> ui;
    
    friend Engine<C>;
    
public:
    virtual void init() {}
    virtual void physics() {}
    virtual void render() {}
    
    virtual ~Scene() = default;
};

template<typename C>
class Engine {
    bool quit_flag = false;
    bool reset = false;
    
public:
    struct GameWindow: public Window {
        Engine *engine;
        
        bool on_keydown(WPARAM key, LPARAM lParam) {
            return engine->on_keydown(key, lParam);
        }
        
        void on_destroy() override {
            engine->on_destroy();
            drop(engine->device); // release the device before window is destroyed
            PostQuitMessage(0);
        }
        
        using Window::Window;
    };
    
    enum SpriteType {
        ui_border,
        ui_background,
        general,
        pointer,
        _sprite_count,
    };
    
    Sprite create_sprite() { return panic_if_failed(Sprite(device, rect_texture), 11, "Failed to create sprite"); }
    
    Vec2i resolution;
    
    WindowClass window_class;
    GameWindow window;
    Device device = panic_if_failed(Device(window, resolution), 10, "Failed to create device");
    Rc<Texture> rect_texture = Sprite::create_rect_texture(device);
    UI::Drawer ui_drawer = {
        [&]() { return create_sprite(); },
        [&]() { return panic_if_failed(Font(device, TEXT("Arial"), Vec2(0, 25), 0), 12, "Failed to create font"); },
    };
    Sprite sprite = create_sprite();
    Keyboard keyboard = panic_if_failed(Keyboard(window, false, false), 13, "Failed to create keyboard");
    Mouse mouse = panic_if_failed(Mouse(window, false, true), 14, "Failed to create mouse");
    Vec2i mouse_pos = Vec2();
    Fmod fmod = panic_if_failed(Fmod(1000), 15, "Failed to create audio");
    Timer timer;
    Color background = Colors::BLACK;
    
    std::vector<Rc<Scene<C>>> scenes;
    
    Rc<Scene<C>> current_scene = nullptr;
    Rc<Texture> pointer_texture;
    
    Engine(LPCTSTR name, Vec2i pos, Vec2i size, int fps, int show):
        resolution(size),
        window_class(WindowClass(name)),
        window(GameWindow(window_class, name, pos, size, show)),
        timer(Timer(fps))
    {
        window.engine = this;
    }
    
    virtual bool on_keydown(WPARAM, LPARAM) { return false; }
    virtual void on_destroy() {}
    
    void quit() { quit_flag = true; }
    
    void toggle_fullscreen() { reset = true; }
    
    double delta() const { return timer.delta(); }
    int frame() const { return timer.frame(); }
    
    virtual void pre_physics() {
        if (!keyboard) keyboard.retry();
        if (!mouse) mouse.retry();
        
        keyboard.update();
        mouse.update();
        
        if (mouse) mouse_pos += mouse.delta();
        
        mouse_pos.x = std::clamp(mouse_pos.x, 0, resolution.x);
        mouse_pos.y = std::clamp(mouse_pos.y, 0, resolution.y);
        
        if (current_scene && current_scene->ui) {
            UI& ui = *current_scene->ui;
            
            ui.calc_min_size(ui_drawer);
            ui.update({}, resolution);
            ui.hover(mouse_pos);
            if (mouse && mouse.left_click() && ui.click(mouse_pos))
                ; // todo consume click
        }
    }
    
    virtual void physics() { if (current_scene) current_scene->physics(); }
    
    virtual void pre_render() {
        fmod.update();
        device.clear(background);
        device.begin();
        sprite.begin();
        ui_drawer.begin();
    }
    
    virtual void render() { if (current_scene) current_scene->render(); }
    
    virtual void render_ui() {
        if (current_scene && current_scene->ui) {
            UI& ui = *current_scene->ui;
            ui.calc_min_size(ui_drawer);
            ui.update({}, resolution);
            ui_drawer.draw(ui);
        }
        
        if (pointer_texture) sprite.draw(*pointer_texture, mouse_pos);
    }
    
    virtual void post_render() {
        ui_drawer.end();
        sprite.end();
        device.end();
        
        if (reset) {
            reset = false;
            ui_drawer.lost();
            sprite.lost();
            device.set_fullscreen(!device.is_fullscreen());
            ui_drawer.reset();
            sprite.reset();
        }
        
        device.present();
    }
    
    virtual void run(C& context) {
        MSG msg;
        ZeroMemory(&msg, sizeof(msg));
        
        while (true) {
            while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                //	Receive a quit message
                if (msg.message == WM_QUIT) return;
                //	Translate the message 
                TranslateMessage(&msg);
                //	Send message to your window procedure
                DispatchMessage(&msg);
            }
            
            #define QUIT if (quit_flag) return;
            
            QUIT
            
            Rc<Scene<C>> scene;
            
            if (!scenes.empty()) scene = scenes.back();
            
            if (scene != current_scene) {
                if (scene && !scene->engine) {
                    scene->engine = this;
                    scene->context = &context;
                    scene->init();
                }
                current_scene = scene;
            }
            
            timer.update();
            
            if (frame() > 0) {
                pre_physics(); QUIT
                physics(); QUIT
            }
            
            pre_render(); QUIT
            render(); QUIT
            render_ui(); QUIT
            post_render(); QUIT
            
            #undef QUIT
        }
    }
};

#endif
