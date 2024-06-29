#pragma once
#include "Map_Object.h"
class Spike :
    public Map_Object
{
private:
	double move_timer = 0;
	Vec2 rotate_center_pos{ 0,0 };//回転するときの中心座標
	Vec2 player_pos{ 0,0 };
public:
	Spike(int x, int y, int m_type,int move_amount = 0, int w = 64, int h = 64);
	void update(double scene_delta_time, Vec2);
	void draw(double, double, bool now_map_make);
	void move();
	void move1();
	void move2();
	void move3();
	void move4();
	void move5();
	void reset();
};

