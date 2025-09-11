
#ifndef LINE_HPP
#define LINE_HPP

#include <span>
#include "dependencies.hpp"
#include "device.hpp"

class Line: public HResult, public Drawer {
    Box<ID3DXLine, RELEASE(ID3DXLine)> line;
    
public:
	using Point = D3DXVECTOR2;

    Line(const Device &device) {
		LPD3DXLINE p;
        result = D3DXCreateLine(&*device, &p);
		line = p;
    }
    
	void begin() override { result = line->Begin(); }
	void end() override { result = line->End(); }
	
	Line& draw(std::span<Point> points, Color color = Colors::BLACK) {
		result = line->Draw(points.data(), points.size(), color.d3d());
		
		return *this;
	}
	
	Line& draw_rect(Vec2f pos, Vec2f size, Color color = Colors::BLACK) {
		std::array<Point, 5> points { pos, pos + Vec2f(size.x, 0), pos + size, pos + Vec2f(0, size.y), pos };
		return draw(points, color);
	}
	
	Line& draw_rect_offset(Vec2f pos, Vec2f size, Color color = Colors::BLACK) {
		return draw_rect(pos - Vec2(2, 2), size + Vec2(3, 3), color);
	}
	
	
	void lost() { result = line->OnLostDevice(); }
	void reset() { result = line->OnResetDevice(); }
	
	WRAP(ID3DXLine, line.get());
};

#endif
