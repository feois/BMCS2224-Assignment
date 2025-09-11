
#ifndef GAME_HPP
#define GAME_HPP

#include "window.hpp"
#include "device.hpp"
#include "sprite.hpp"
#include "line.hpp"
#include "utils.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "audio.hpp"
#include "timer.hpp"
#include "animation.hpp"
#include "texture_locker.hpp"
#include "font.hpp"
#include "ui.hpp"
#include "physics.hpp"
#include "sprite_sheet.hpp"
#include "direction.hpp"
#include <vector>

class Game;

class Scene {
protected:
    Game *game = nullptr;
    bool initialized = false;
    
    friend Game;
    
    void try_init() {
        if (!initialized) {
            init();
            initialized = true;
        }
    }
    
public:
    virtual void init() {}
    virtual void init_ui() {}
    virtual void physics() {}
    virtual void render() {}
    
    virtual ~Scene() = default;
};

template<typename C>
struct SceneContext: public Scene {
    C& context() { return *dynamic_cast<C *>(game); }
};

class Game {
    bool quit_flag = false;
    bool reset = false;
    
public:
    struct GameWindow: public Window {
        Game *game;
        
        bool on_keydown(WPARAM key, LPARAM lParam) {
            return game->on_keydown(key, lParam);
        }
        
        void on_destroy() override {
            game->on_destroy();
            drop(game->device); // release the device before window is destroyed
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
    
    Vec2i resolution;
    
    WindowClass window_class;
    GameWindow window;
    Device device;
    std::array<Sprite, _sprite_count> sprites;
    Font font;
    Line line;
    Keyboard keyboard;
    Mouse mouse;
    Vec2i mouse_pos;
    Fmod fmod;
    Timer timer;
    Color background = Colors::BLACK;
    
    Box<Texture> rect_base;
    std::vector<std::shared_ptr<UI>> ui_elements;
    std::vector<std::shared_ptr<Scene>> scenes;
    
    std::shared_ptr<Scene> current_scene = nullptr;
    std::shared_ptr<Texture> pointer_texture;
    
    Game(LPCTSTR name, Vec2i pos, Vec2i size, int fps, int show):
        resolution(size),
        window_class(WindowClass(name)),
        window(GameWindow(window_class, name, pos, size, show)),
        device(panic_if_failed(Device(window, size), 10, "Failed to create device")),
        sprites(array_from_fn<Sprite, _sprite_count>([&]() { return panic_if_failed(Sprite(device), 11, "Failed to create sprite"); })),
        font(panic_if_failed(Font(device, TEXT("Arial"), Vec2(0, 25), 0), 12, "Failed to create font")),
        line(panic_if_failed(Line(device), 13, "Failed to create line")),
        keyboard(panic_if_failed(Keyboard(window, false, false), 14, "Failed to create keyboard")),
        mouse(panic_if_failed(Mouse(window, false, true), 15, "Failed to create mouse")),
        fmod(panic_if_failed(Fmod(1000), 16, "Failed to create audio")),
        timer(Timer(fps))
    {
        window.game = this;
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
        
        for (std::shared_ptr<UI>& ui : ui_elements) {
            ui->hover = mouse_pos.in_rect(ui->position(), ui->size());
            
            if (ui->hover && mouse && mouse.left_click() && ui->on_click()) break;
        }
    }
    
    virtual void physics() { if (current_scene) current_scene->physics(); }
    
    virtual void pre_render() {
        fmod.update();
    }
    
    virtual void render() { if (current_scene) current_scene->render(); }
    
    virtual void render_ui() {
        for (std::shared_ptr<UI> &ui : ui_elements) ui->draw(*this);
        
        font.flush();
        
        if (pointer_texture) sprites[pointer].draw(*pointer_texture, mouse_pos);
    }
    
    virtual void begin() {
        device.clear(background);
        device.begin();
        for (auto& sprite : sprites) sprite.begin();
        line.begin();
        font.begin();
    }
    
    virtual void end() {
        for (auto& sprite : sprites) sprite.end();
        font.end();
        line.end();
        device.end();
        
        if (reset) {
            line.lost();
            font.lost();
            for (auto& sprite : sprites) sprite.lost();
            
            device.set_fullscreen(!device.is_fullscreen());
            reset = false;
            
            for (auto &sprite : sprites) sprite.reset();
            font.reset();
            line.reset();
        }
        
        device.present();
    }
    
    virtual void run() {
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
            
            std::shared_ptr<Scene> scene;
            
            if (!scenes.empty()) scene = scenes.back();
            
            if (scene != current_scene) {
                ui_elements.clear();
                if (scene) {
                    scene->game = this;
                    scene->try_init();
                    scene->init_ui();
                }
                current_scene = scene;
            }
            
            timer.update();
            
            if (frame() > 0) {
                pre_physics(); QUIT
                physics(); QUIT
            }
            
            pre_render(); QUIT
            begin(); QUIT
            render(); QUIT
            render_ui(); QUIT
            end(); QUIT
            
            #undef QUIT
        }
    }
    
    Sprite& draw_rect(Sprite &sprite, Vec2f pos, Vec2f size, Color color = Colors::BLACK) {
        if (!rect_base) {
            rect_base = new Texture(device, Vec2(1, 1));
            TextureWriter(*rect_base).set_pixel(Vec2(), Colors::WHITE);
        }
        
        return sprite.draw(*rect_base, Transform() + Scale(size) + Translate(pos), color);
    }
    
    Sprite& draw_rect(SpriteType type, Vec2f pos, Vec2f size, Color color = Colors::BLACK) {
        return draw_rect(sprites[type], pos, size, color);
    }
};

#endif
