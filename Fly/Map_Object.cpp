#include "stdafx.h"
#include "Map_Object.h"

//オブジェクトでもこまごました奴は全部ここに書く

bool Map_Object::created_block()
{

	if (change == false)
	{
		
		change = true;
		can_hit = true;

		object_w = 64;
		object_h = 64;

		hit_r = RectF{ pos,object_w,object_h };

		return true;
	}
	else
	{
		return false;
	}
	
}

Player_Spon::Player_Spon(int x, int y)
{
	can_hit = false;

	object_kind = Object_Kind_Player_Spon;
	first_object_kind = object_kind;

	m_pos = Vec2{ x,y };
	pos = m_pos*Vec2{ 64,64 };
	s_pos = pos;
	object_w = 64;
	object_h = 64;
	hit_r = RectF{ pos,object_w,object_h };
}

void Player_Spon::draw(double c_x, double c_y, bool now_map_make)
{
	Vec2 tex_pos = m_pos*64- Vec2{ c_x - 640,c_y - 480 };
	RectF(tex_pos, 64, 64).draw(Palette::White);
}


Enemy_Spon::Enemy_Spon(int x, int y,int k)
{
	can_hit = false;

	object_kind = Object_Kind_Enemy_Spon;
	first_object_kind = object_kind;

	enemy_kind = k;
	m_pos = Vec2{ x,y };
	pos = m_pos * Vec2{ 64,64 };
	s_pos = pos;
	object_w = 64;
	object_h = 64;
	hit_r = RectF{ pos,object_w,object_h };
}

void Enemy_Spon::draw(double c_x, double c_y, bool now_map_make)
{
	if (now_map_make)
	{
		Vec2 tex_pos = m_pos * 64 - Vec2{ c_x - 640,c_y - 480 };
		RectF(tex_pos, 64, 64).draw(Palette::Purple);
	}
}



Goal::Goal(int x, int y)
{
	can_hit = false;

	object_kind = Object_Kind_Goal;
	first_object_kind = object_kind;

	m_pos = Vec2{ x,y };
	pos = m_pos * Vec2{ 64,64 };
	s_pos = pos;
	object_w = 64;
	object_h = 64;
	hit_r = RectF{ pos,object_w,object_h };
}

void Goal::draw(double c_x, double c_y, bool now_map_make)
{
	Vec2 tex_pos = m_pos * 64 - Vec2{ c_x - 640,c_y - 480 };

	TextureAsset(U"Goal").draw(tex_pos);
	//RectF(tex_pos, 64, 64).draw(Palette::Yellow);
}

Star::Star(int x, int y)
{
	can_hit = false;

	object_kind = Object_Kind_Star;
	first_object_kind = object_kind;

	m_pos = Vec2{ x,y };
	pos = m_pos * Vec2{ 64,64 } + Vec2{32,32};//星の中心座標
	s_pos = pos;
	object_w = 50;
	object_h = 50;
	hit_r = RectF{ pos - Vec2{object_w/2,object_h/2},object_w,object_h };
}

void Star::update(double scene_del, Vec2 p)
{
	color_change_timer += scene_del;

	color = 30 + 25 * sin(360_deg*color_change_timer);

	if (color_change_timer >= 360) { color_change_timer = 0; };

	/*
	if (color_change_timer >= color_change_span)
	{
		color_change_timer = 0;

	

		color %= int(70);

		//color = Max(30, color);
	}
	*/
}

void Star::draw(double c_x, double c_y, bool now_map_make)
{
	Vec2 tex_pos = hit_r.pos - Vec2{ c_x - 640,c_y - 480 };



	if (can_see)
	{
		Shape2D::Star(object_w / 2, tex_pos + Vec2{ object_w / 2,object_h / 2 }).draw(HSV(color,1,0.97,1)).drawFrame(0,Palette::Black);

	}
	//RectF(tex_pos, object_w, object_h).drawFrame(2,Palette::Black);
}

void Star::reset()
{
	can_see = true;
}
