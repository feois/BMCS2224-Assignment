
#ifndef FONT_HPP
#define FONT_HPP

#include "direct3d.hpp"
#include "sprite.hpp"
#include <engine/alignment.hpp>

struct TextConfig: public HResult {
	#define tc_member(t, n, v) t n = v; \
		TextConfig set_##n(t n) const { auto tc = *this; tc.n = n; return tc; }
	
	tc_member(Vec2i, size, Vec2i());
	tc_member(Alignment, alignment, Alignment(Alignment::Top, Alignment::Left))
	tc_member(Color, color, Colors::BLACK);
    tc_member(Transform, transform, Transform());
	
	#undef tc_member
	
	#define tc_flag(n, d) bool n = d; \
		TextConfig set_##n() { auto tc = *this; tc.n = true; return tc; } \
		TextConfig unset_##n() { auto tc = *this; tc.n = false; return tc; }
	
	tc_flag(single_line, false);
	tc_flag(clipping, true);
	tc_flag(wrapping, false);
	tc_flag(auto_size, true);
	
	#undef tc_flag
	
    DWORD flags() const;
};

class Font: public HResult, public Renderer {
    Box<ID3DXFont, RELEASE(ID3DXFont)> font;
    Sprite sprite;
    
public:
    Font(const Device &device, LPCTSTR name, Vec2i size, int weight, bool italic = false);
	
    void begin() override;
	void end() override;
	
    Vec2i get_size(TStr str, Vec2i pos, TextConfig config = {});
    
	Font& write(TStr str, Vec2i pos, TextConfig config = {});
	
    void lost() override;
    void reset() override;
    
    WRAP(ID3DXFont, font.get());
};

#endif
