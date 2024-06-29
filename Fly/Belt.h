#pragma once
#include "Map_Object.h"
class Belt :
    public Map_Object
{
private:
	double move_timer = 0;
	double animation_timer = 0;//アニメーションの切り替わりよう
	double animation_span  = 0.3;
	int animation_index{ 1 };

public:
	Belt(int x, int y, int m_type, int m_amount = 0, int w = 64, int h = 64);
	void update(double scene_delta_time, Vec2);
	void draw(double, double, bool now_map_make);
	void reset();
};

