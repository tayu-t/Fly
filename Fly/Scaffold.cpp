#include "stdafx.h"
#include "Scaffold.h"

Scaffold::Scaffold(int m_x, int m_y, int m_type, int m_amount,int w,int h)
{
	m_pos = Vec2{ m_x,m_y };
	pos = Vec2{ m_x,m_y }*Vec2{64,64};
	pre_pos = pos;
	s_pos = pos;

	move_type = m_type;

	if (move_type == 3)
	{
		//回転
		rotate_center_pos = Vec2{ 64,64 }*Vec2{ m_x,m_y + m_amount } + Vec2{ 32,32 };
	}

	move_amount = m_amount;

	init_stop_time = 0.5;//最初の瞬間はブロックが動かない

	object_w = w;
	object_h = h;
	object_kind = Object_Kind::Object_Kind_Scaffold;
	first_object_kind = object_kind;

	hit_r = RectF{ pos,object_w,object_h };
}

void Scaffold::update(double scene_deltatime, Vec2 p_pos)
{
	pre_pos = pos;
	scene_del = scene_deltatime;


	player_pos = p_pos;

	if (init_stop_time > 0)
	{
		init_stop_time -= scene_del;
		fu_speed = { 0,0 };
	}
	else
	{
		move();
	}


	delta_pos = pos - pre_pos;
	speed = delta_pos / scene_del;

	hit_r = RectF{ pos,object_w,object_h };
}


void Scaffold::move()
{
	switch (move_type)
	{
	case 0:
		break;
	case 1:
		move1();
		break;
	case 2:
		move2();
		break;
	case 3:
		move3();
		break;
	case 4:
		move4();
		break;
	case 5:
		move5();
		break;
	default:
		break;
	}
}

void Scaffold::move1()
{

	if (move_switch)
	{
		//右に移動
		pos.x += 64 * move_amount * scene_del;
		fu_speed = Vec2{ 1,0 }*64 * move_amount;

		if (pos.x >= s_pos.x + 64 * move_amount)
		{
			fu_speed = Vec2{ -1,0 }*64 * move_amount;
			move_switch = false;
		}
	}
	else
	{
		//左に移動
		pos.x -= 64 * move_amount * scene_del;
		fu_speed = Vec2{ -1,0 }*64 * move_amount;
		if (pos.x <= s_pos.x - 64 * move_amount)
		{
			fu_speed = Vec2{ 1,0 }*64 * move_amount;
			move_switch = true;
		}
	}
}

void Scaffold::move2()
{

	if (move_switch)
	{

		//下に移動

		pos.y += 64 * move_amount * scene_del;

		fu_speed = Vec2{0,1}*64 * move_amount;

		if (pos.y >= s_pos.y + 64 * move_amount)
		{
			fu_speed = Vec2{ 0,-1 }*64 * move_amount;

			move_switch = false;
		}
	}
	else
	{
		//上に移動
		pos.y -= 64 * move_amount * scene_del;
		fu_speed = Vec2{ 0,-1 }*64 * move_amount;
		if (pos.y <= s_pos.y - 64 * move_amount)
		{
			fu_speed = Vec2{ 0,1 }*64 * move_amount;
			move_switch = true;
		}
	}
}

void Scaffold::move3()
{

	//円運動
	
	move_timer += scene_del;

	Vec2 rect_center_pos;//四角形の中心座標

	//回転する
	rect_center_pos = rotate_center_pos + 64 * move_amount * Vec2{ sin(30_deg * move_timer),-cos(30_deg * move_timer) };

	pos = rect_center_pos - Vec2{ object_w / 2,object_h / 2 };

	fu_speed = (3.14/6)*64 * move_amount * Vec2{ cos(30_deg * (move_timer+scene_del)),sin(30_deg * (move_timer+scene_del)) };

	if (move_timer >= 60)
	{
		move_timer = 0;
	}
	
}

void Scaffold::move4()
{
	if ((pos - player_pos).length() <= 128)
	{

		double speed_alfa = 1.3;

		if (move_switch)
		{
			//右に移動
			pos.x += speed_alfa * 64 * move_amount * scene_del;
			fu_speed = speed_alfa * Vec2{ 1,0 }*64 * move_amount;

			if (pos.x >= s_pos.x + 64 * move_amount)
			{
				fu_speed = speed_alfa * Vec2{ -1,0 }*64 * move_amount;
				move_switch = false;
			}
		}
		else
		{
			//左に移動
			pos.x -= speed_alfa * 64 * move_amount * scene_del;
			fu_speed = speed_alfa * Vec2{ -1,0 }*64 * move_amount;
			if (pos.x <= s_pos.x - 64 * move_amount)
			{
				fu_speed = speed_alfa * Vec2{ 1,0 }*64 * move_amount;
				move_switch = true;
			}
		}
	}
}

void Scaffold::move5()
{
	if ((pos - player_pos).length() <= 128)
	{
		double speed_alfa = 1.3;

		if (move_switch)
		{

			//下に移動

			pos.y += speed_alfa * 64 * move_amount * scene_del;

			fu_speed = speed_alfa * Vec2{ 0,1 }*64 * move_amount;

			if (pos.y >= s_pos.y + 64 * move_amount)
			{
				fu_speed = speed_alfa * Vec2{ 0,-1 }*64 * move_amount;

				move_switch = false;
			}
		}
		else
		{
			//上に移動
			pos.y -= speed_alfa * 64 * move_amount * scene_del;
			fu_speed = speed_alfa * Vec2{ 0,-1 }*64 * move_amount;
			if (pos.y <= s_pos.y - 64 * move_amount)
			{
				fu_speed = speed_alfa * Vec2{ 0,1 }*64 * move_amount;
				move_switch = true;
			}
		}
	}
}

void Scaffold::draw(double c_x, double c_y, bool now_map_make)
{
	Vec2 tex_pos = pos - Vec2{ c_x - 640,c_y - 480 };

	TextureAsset(U"Scaffold").draw(tex_pos);


	if (move_type > 0 && now_map_make)
	{
		FontAsset(U"sentence")(U"{}\n{}\n{}"_fmt(m_pos, move_type, move_amount)).drawAt(tex_pos + Vec2{ 32,32 });

		switch (move_type)
		{
		case 1:
			Line(tex_pos + Vec2{ object_w / 2,object_h / 2 } - Vec2{ 64 * move_amount,0 }, tex_pos + Vec2{ object_w / 2,object_h / 2 } + Vec2{ 64 * move_amount,0 }).draw(3, Palette::Yellow);
			break;
		case 2:
			Line(tex_pos + Vec2{ object_w / 2,object_h / 2 } - Vec2{ 0, 64 * move_amount }, tex_pos + Vec2{ object_w / 2,object_h / 2 } + Vec2{ 0,64 * move_amount }).draw(3, Palette::Yellow);
			break;
		case 3:
			Vec2 tex_pos = rotate_center_pos - Vec2{ c_x - 640,c_y - 480 };
			Circle(tex_pos, 64 * move_amount).drawFrame(3, Palette::Yellow);
			break;
		case 4:
			Line(tex_pos + Vec2{ object_w / 2,object_h / 2 } - Vec2{ 64 * move_amount,0 }, tex_pos + Vec2{ object_w / 2,object_h / 2 } + Vec2{ 64 * move_amount,0 }).draw(3, Palette::Yellow);
			break;
		case 5:
			Line(tex_pos + Vec2{ object_w / 2,object_h / 2 } - Vec2{ 0, 64 * move_amount }, tex_pos + Vec2{ object_w / 2,object_h / 2 } + Vec2{ 0,64 * move_amount }).draw(3, Palette::Yellow);
			break;
		default:
			break;
		}
	}
}

void Scaffold::reset()
{
	hit_r = RectF{ pos,object_w,object_h };
	move_timer = 0;
	move_switch = false;
	pos = s_pos;
	pre_pos = pos;
	speed = { 0,0 };
	fu_speed = { 0,0 };
	init_stop_time = 0.7;//最初の瞬間はブロックが動かない
}

