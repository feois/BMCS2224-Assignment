
#ifndef UI_HPP
#define UI_HPP

#include <direct3d/font.hpp>
#include <engine/texture_rect.hpp>
#include <ranges>
#include <functional>

class UI {
    Vec2i position {};
    Vec2i size {};
    
public:
    Vec2i min_size {};
    
    constexpr bool in_rect(Vec2i pos) const {
        return pos.x >= position.x && pos.x <= position.x + size.x && pos.y >= position.y && pos.y <= position.y + size.y;
    }
    
    struct Drawer;
    
    constexpr Vec2i get_position() const { return position; }
    constexpr Vec2i get_size() const { return size; }
    
    virtual void calc_min_size(Drawer &drawer) = 0;
    virtual void update(Vec2i pos, Vec2i max_size) { position = pos; size = max_size; }
    virtual void draw(Drawer &drawer, Renderer* z_order) = 0;
    virtual bool hover([[maybe_unused]] Vec2i pos) { return false; }
    virtual bool click([[maybe_unused]] Vec2i pos) { return false; }
    
    constexpr static Color highlight(Color color) { return color.interpolate(Colors::WHITE, 0.35); }
    
    virtual ~UI() = default;
    
    struct Drawer: Renderer {
        Sprite border, background, texture;
        Font text;
        
        Drawer(std::function<Sprite()> sprite_creator, std::function<Font()> font_creator)
        : border(sprite_creator()), background(sprite_creator()), texture(sprite_creator())
        , text(font_creator()) {
            renderers = {
                &border,
                &background,
                &texture,
                &text,
            };
        }
        
        void begin() override {}
        void end() override {}
        void lost() override { for (auto& r : renderers) r->lost(); }
        void reset() override { for (auto& r : renderers) r->reset(); }
        
        void draw(UI& ui) {
            for (auto renderer : renderers) {
                renderer->begin();
                ui.draw(*this, renderer);
                renderer->end();
            }
        }
        
    private:
        std::vector<Renderer *> renderers {};
    };
};

struct Wrapper: public UI {
    Box<UI> child;
    
    Wrapper(Box<UI> &&child): child(std::move(child)) {}
    
    void draw(Drawer &drawer, Renderer* z_order) override { child->draw(drawer, z_order); }
    bool hover(Vec2i pos) override { return child->hover(pos); }
    bool click(Vec2i pos) override { return child->click(pos); }
};

struct Stretch {
    float stretch;
    Box<UI> ui;
    
    Stretch(float stretch, Box<UI>&& ui): stretch(stretch), ui(std::move(ui)) {}
    Stretch(Box<UI>&& ui): stretch(0), ui(std::move(ui)) {}
};

struct Container: public UI {
    float stretch_sum = 0;
    std::vector<Stretch> children {};
    
    Container(std::vector<Stretch> &&children): children(std::move(children)) {}
    
    template<typename... Args>
    Container(Args&&... args) {
        children.reserve(sizeof...(args));
        (children.emplace_back(std::forward<Args>(args)), ...);
    }
    
    void draw(Drawer &drawer, Renderer* z_order) override { for (auto& child : children) child.ui->draw(drawer, z_order); }
    bool hover(Vec2i pos) override {
        for (auto& child : children)
            if (child.ui->hover(pos)) return true;
        return false;
    }
    bool click(Vec2i pos) override {
        for (auto& child : children)
            if (child.ui->click(pos)) return true;
        return false;
    }
};

template<typename T>
struct CompositeUI: public UI {
    T ui;
    
    CompositeUI(T&& ui): ui(std::move(ui)) {}
    
    void calc_min_size(Drawer &drawer) override { ui.calc_min_size(drawer); min_size = ui.min_size; }
    void update(Vec2i pos, Vec2i max_size) override { ui.update(pos, max_size); UI::update(pos, max_size); }
    void draw(Drawer &drawer, Renderer* z_order) override { ui.draw(drawer, z_order); }
    bool hover(Vec2i pos) override { return ui.hover(pos); }
    bool click(Vec2i pos) override { return ui.in_rect(pos) && on_click(); }
    virtual bool on_click() { return false; }
};

struct VContainer: public Container {
    using Container::Container;

    void calc_min_size(Drawer &drawer) override {
        stretch_sum = 0;
        min_size = Vec2();
        
        for (auto& child : children) {
            child.ui->calc_min_size(drawer);
            min_size.x = std::max(min_size.x, child.ui->min_size.x);
            min_size.y += child.ui->min_size.y;
            stretch_sum += child.stretch;
        }
    }
    void update(Vec2i pos, Vec2i max_size) override {
        auto extra = max_size.y - min_size.y;
        auto p = pos;
        
        for (auto& child : children) {
            auto s = Vec2i(max_size.x, child.ui->min_size.y);
            if (child.stretch > 0) s += Vec2(0., extra * child.stretch / stretch_sum);
            child.ui->update(p, s);
            p.y += s.y;
        }
        
        UI::update(pos, max_size);
    }
};

struct HContainer: public Container {
    using Container::Container;

    void calc_min_size(Drawer &drawer) override {
        stretch_sum = 0;
        min_size = Vec2();
        
        for (auto& child : children) {
            child.ui->calc_min_size(drawer);
            min_size.y = std::max(min_size.y, child.ui->min_size.y);
            min_size.x += child.ui->min_size.x;
            stretch_sum += child.stretch;
        }
    }
    void update(Vec2i pos, Vec2i max_size) override {
        auto extra = max_size.x - min_size.x;
        auto p = pos;
        
        for (auto& child : children) {
            auto s = Vec2i(child.ui->min_size.x, max_size.y);
            if (child.stretch > 0) s += Vec2(extra * child.stretch / stretch_sum, 0.);
            child.ui->update(p, s);
            p.x += s.x;
        }
        
        UI::update(pos, max_size);
    }
};

struct StackContainer: public UI {
    std::vector<Box<UI>> children;
    
    StackContainer(std::vector<Box<UI>> &&children): children(std::move(children)) {}
    
    template<typename... Args>
    StackContainer(Args&&... args) {
        children.reserve(sizeof...(args));
        (children.emplace_back(std::forward<Args>(args)), ...);
    }
    
    void calc_min_size(Drawer &drawer) override {
        min_size = Vec2();
        
        for (auto& child : children) {
            child->calc_min_size(drawer);
            min_size.x = std::max(min_size.x, child->min_size.x);
            min_size.y = std::max(min_size.y, child->min_size.y);
        }
    }
    
    void update(Vec2i pos, Vec2i max_size) override {
        for (auto& child : children) child->update(pos, max_size);
        UI::update(pos, max_size);
    }
    
    void draw(Drawer &drawer, Renderer* z_order) override { for (auto& child : children) child->draw(drawer, z_order); }
    bool hover(Vec2i pos) override {
        for (auto& child : std::views::reverse(children))
            if (child->hover(pos)) return true;
        return false;
    }
    bool click(Vec2i pos) override {
        for (auto& child : std::views::reverse(children))
            if (child->click(pos)) return true;
        return false;
    }
};

struct AlignContainer: public Wrapper {
    Alignment alignment {};
    bool vertical_align = true;
    bool horizontal_align = true;
    
    AlignContainer(Alignment alignment, Box<UI> &&child): Wrapper(std::move(child)), alignment(alignment) {}
    AlignContainer(Alignment::Vertical alignment, Box<UI> &&child): Wrapper(std::move(child)) {
        this->alignment.vertical = alignment;
        horizontal_align = false;
    }
    AlignContainer(Alignment::Horizontal alignment, Box<UI> &&child): Wrapper(std::move(child)) {
        this->alignment.horizontal = alignment;
        vertical_align = false;
    }
    
    void calc_min_size(Drawer &drawer) override { child->calc_min_size(drawer); min_size = child->min_size; }
    void update(Vec2i pos, Vec2i max_size) override {
        auto p = pos;
        auto s = max_size;
        
        if (vertical_align) {
            switch (alignment.vertical) {
                case Alignment::Top: break;
                case Alignment::VCenter:
                    p.y += max_size.y / 2;
                    p.y -= min_size.y / 2;
                    break;
                case Alignment::Bottom:
                    p.y += max_size.y;
                    p.y -= min_size.y;
                    break;
            }
            
            s.y = min_size.y;
        }
        
        if (horizontal_align) {
            switch (alignment.horizontal) {
                case Alignment::Left: break;
                case Alignment::Center:
                    p.x += max_size.x / 2;
                    p.x -= min_size.x / 2;
                    break;
                case Alignment::Right:
                    p.x += max_size.x;
                    p.x -= min_size.x;
                    break;
            }
            
            s.x = min_size.x;
        }
        
        child->update(p, s);
        
        UI::update(pos, max_size);
    }
};

struct Margin: public UI {
    Margin(Vec2i size) { min_size = size; }
    
    void calc_min_size([[maybe_unused]] Drawer &drawer) override {}
    void draw([[maybe_unused]] Drawer &drawer, [[maybe_unused]] Renderer* z_order) override {}
};

struct Padding: public Wrapper {
    Vec2i padding;
    
    Padding(Vec2i padding, Box<UI> &&child): Wrapper(std::move(child)), padding(padding) {}
    
    void calc_min_size(Drawer &drawer) override {
        child->calc_min_size(drawer);
        min_size = child->min_size + padding * 2;
    }
    void update(Vec2i pos, Vec2i max_size) override { child->update(pos + padding, max_size - padding * 2); UI::update(pos, max_size); }
};

struct Panel: public Wrapper {
    Color background;
    int border_width = 0;
    Color border = Colors::BLACK;
    
    Panel(Color background, int border_width, Color border, Box<UI> &&child)
    : Wrapper(std::move(child)), background(background), border_width(border_width), border(border) {}
    
    Panel(Color background, Box<UI> &&child): Wrapper(std::move(child)), background(background) {}
    
    constexpr Vec2i offset() const { return Vec2i(border_width, border_width); }
    
    void calc_min_size(Drawer &drawer) override {
        child->calc_min_size(drawer);
        min_size = child->min_size + offset() * 2;
    }
    void update(Vec2i pos, Vec2i max_size) override {
        child->update(pos + offset(), max_size - offset() * 2);
        UI::update(pos, max_size);
    }
    void draw(Drawer &drawer, Renderer* z_order) override {
        if (z_order == &drawer.border) {
            if (border_width > 0) drawer.border.draw_rect(get_position().to_f(), get_size().to_f(), border);
        }
        
        if (z_order == &drawer.background) {
            drawer.background.draw_rect((get_position() + offset()).to_f(), get_size().to_f(), background);
        }
        
        child->draw(drawer, z_order);
    }
};

struct Text: public UI {
    TStr text;
    TextConfig config;
    
    Text(TStr text, TextConfig config): text(text), config(config) {}
    
    void calc_min_size(Drawer &drawer) override { min_size = drawer.text.get_size(text, Vec2(), config); }
    void draw(Drawer &drawer, Renderer* z_order) override {
        if (z_order == &drawer.text) drawer.text.write(text, get_position(), config);
    }
};

struct TextureUI: public UI {
    TextureRect texture;
    
    TextureUI(TextureRect texture): texture(texture) {}
    
    void calc_min_size([[maybe_unused]] Drawer &drawer) override { min_size = texture.size(); }
    void draw(Drawer &drawer, Renderer* z_order) override {
        if (z_order == &drawer.texture) drawer.texture.draw(texture, get_position());
    }
};

struct Label: public CompositeUI<Panel> {
    Text* text_ui;
    
    Label(TStr text, Color background, int border_width, Color border, Vec2i padding, TextConfig config)
    : CompositeUI {
        Panel {
            background,
            border_width,
            border,
            new Padding {
                padding,
                text_ui = new Text {
                    text,
                    config,
                },
            },
        },
    } {}
    
    Label& set_text(TStr text) { text_ui->text = text; return *this; }
};

struct Button: public CompositeUI<Panel> {
    std::function<void ()> callback;
    bool enabled = true;
    bool hovered = false;
    
    Button(Color background, int border_width, Color border, Vec2i padding, Box<UI>&& ui, std::function<void ()> callback)
    : CompositeUI {
        Panel {
            background,
            border_width,
            border,
            new Padding {
                padding,
                std::move(ui),
            }
        }
    }, callback(callback) {}
    
    void draw(Drawer &drawer, Renderer* z_order) override {
        if (z_order == &drawer.background) {
            auto c = ui.background;
            if (hovered) ui.background = highlight(c);
            ui.draw(drawer, z_order);
            ui.background = c;
        }
        else ui.draw(drawer, z_order);
    }
    
    bool hover(Vec2i pos) override { return hovered = ui.in_rect(pos); }
    
    bool on_click() override {
        if (enabled) callback();
        return enabled;
    }
};

#endif
