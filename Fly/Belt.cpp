#include "stdafx.h"
#include "Belt.h"

Belt::Belt(int m_x, int m_y, int m_type, int m_amount, int w, int h)
{
	m_pos = Vec2{ m_x,m_y };
	pos = Vec2{ m_x,m_y }*Vec2{ 64,64 };
	pre_pos = pos;
	s_pos = pos;


	//move_type
	//0:当たり判定があり右
	//1:当たり判定があり左

	if (move_type == 0 || move_type == 1)
	{
		move_type = m_type;
	}
	else
	{
		move_type = 0;
	}
	

	move_amount = m_amount;

	animation_span = 0.8 - (0.2 * m_amount);

	init_stop_time = 0.5;//最初の瞬間はブロックが動かない

	object_w = w;
	object_h = h;
	object_kind = Object_Kind::Object_Kind_Belt;
	first_object_kind = object_kind;

	hit_r = RectF{ pos,object_w,object_h };
}

void Belt::update(double scene_deltatime, Vec2 p)
{
	pre_pos = pos;
	scene_del = scene_deltatime;

	animation_timer += scene_del;

	
	///一定間隔でアニメーションを切り替える 
	if (animation_timer > animation_span)
	{
		animation_timer = 0;
		animation_index++;
		if (animation_index > 4)
		{
			animation_index = 1;
		}
	}

	if (init_stop_time > 0)
	{
		init_stop_time -= scene_del;
		fu_speed = { 0,0 };
	}
	else
	{
		double fu_speed_x = 30 + 128 * move_amount + 45;
		//回転
		if (move_type == 0)
		{
			//右にプレイヤーを飛ばす
			fu_speed = Vec2{ fu_speed_x,0 };
		}
		else
		{
			//左にプレイヤーを飛ばす
			fu_speed = Vec2{ -fu_speed_x,0 };
		}
	}

	hit_r = RectF{ pos,object_w,object_h };
}


void Belt::draw(double c_x, double c_y, bool now_map_make)
{
	Vec2 tex_pos = pos - Vec2{ c_x - 640,c_y - 480 };

	RectF{ tex_pos,object_w,object_h }.draw(Palette::Green);

	if (move_type == 0)
	{
		//右
		String tex_name = U"Belt_r_{}"_fmt(animation_index);

		TextureAsset(tex_name).draw(tex_pos);
	}
	else
	{
		//左
		String tex_name = U"Belt_l_{}"_fmt(animation_index);

		TextureAsset(tex_name).draw(tex_pos);
	}

	if (now_map_make)
	{

		FontAsset(U"sentence")(U"{}\n{}\n{}"_fmt(m_pos, move_type, move_amount)).drawAt(tex_pos + Vec2{ 32,32 });
	}
	
}

void Belt::reset()
{
	hit_r = RectF{ pos,object_w,object_h };
	move_switch = false;
	pos = s_pos;
	pre_pos = pos;
	speed = { 0,0 };
	fu_speed = { 0,0 };
	init_stop_time = 0.7;//最初の瞬間はブロックが動かない
}
