
#ifndef LABEL_HPP
#define LABEL_HPP

#include "composite_ui.hpp"
#include "panel.hpp"
#include "padding.hpp"
#include "text.hpp"

// draws border and background, add padding and write text
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

#endif
