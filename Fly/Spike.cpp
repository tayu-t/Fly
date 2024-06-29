#include "stdafx.h"
#include "Spike.h"

Spike::Spike(int m_x, int m_y, int m_type,int m_amount, int w, int h)
{
	m_pos = Vec2{ m_x,m_y };
	
	s_pos = Vec2{ m_x,m_y }*Vec2{ 64,64 };
	move_type = m_type;
	move_amount = m_amount;
	object_w = w;
	object_h = h;
	object_kind = Object_Kind::Object_Kind_Spike;
	first_object_kind = object_kind;


	if (move_type == 3)
	{
		//回転
		rotate_center_pos = Vec2{ 64,64 }*Vec2{ m_x,m_y + m_amount } + Vec2{ 32,32 };
	}

	reset();
}

void Spike::update(double scene_deltatime, Vec2 p_pos)
{
	pre_pos = pos;
	scene_del = scene_deltatime;

	player_pos = p_pos;

	if(init_stop_time > 0)
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

void Spike::move()
{
	switch (move_type)
	{
	case 0:
		break;//静止
	case 1:
		move1();
		break;
	case 2 :
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

//横の平行移動
void Spike::move1()
{
	if (move_switch)
	{
		//右に移動
		pos.x += 48 * move_amount * scene_del;
		fu_speed = Vec2{ 1,0 }*48 * move_amount;

		if (pos.x >= s_pos.x + 64 * move_amount)
		{
			fu_speed = Vec2{ -1,0 }*48 * move_amount;
			move_switch = false;
		}
	}
	else
	{
		//左に移動
		pos.x -= 48 * move_amount * scene_del;
		fu_speed = Vec2{ -1,0 }*48 * move_amount;
		if (pos.x <= s_pos.x - 64 * move_amount)
		{
			fu_speed = Vec2{ 1,0 }*48 * move_amount;
			move_switch = true;
		}
	}
}

//縦の平行移動
void Spike::move2()
{
	if (move_switch)
	{

		//下に移動

		pos.y += 48 * move_amount * scene_del;

		fu_speed = Vec2{ 0,1 }*48 * move_amount;

		if (pos.y >= s_pos.y + 64 * move_amount)
		{
			fu_speed = Vec2{ 0,-1 }*48 * move_amount;

			move_switch = false;
		}
	}
	else
	{
		//上に移動
		pos.y -= 48 * move_amount * scene_del;
		fu_speed = Vec2{ 0,-1 }*48 * move_amount;
		if (pos.y <= s_pos.y - 64 * move_amount)
		{
			fu_speed = Vec2{ 0,1 }*48 * move_amount;
			move_switch = true;
		}
	}
}

void Spike::move3()
{


	//円運動

	move_timer += scene_del;

	Vec2 rect_center_pos;//四角形の中心座標

	//回転する
	rect_center_pos = rotate_center_pos + 64 * move_amount * Vec2{ sin(30_deg * move_timer),-cos(30_deg * move_timer) };

	pos = rect_center_pos - Vec2{ object_w / 2,object_h / 2 };

	fu_speed = (3.14 / 6) * 64 * move_amount * Vec2{ cos(30_deg * (move_timer + scene_del)),sin(30_deg * (move_timer + scene_del)) };

	if (move_timer >= 60)
	{
		move_timer = 0;
	}

}

void Spike::move4()
{
	if ((pos - player_pos).length() <= 128)
	{
		
		double speed_alfa = 1.3;


		if (move_switch)
		{
			//右に移動
			pos.x += speed_alfa*48 * move_amount * scene_del;
			fu_speed = speed_alfa * Vec2{ 1,0 }*48 * move_amount;

			if (pos.x >= s_pos.x + 64 * move_amount)
			{
				fu_speed = speed_alfa * Vec2{ -1,0 }*48 * move_amount;
				move_switch = false;
			}
		}
		else
		{
			//左に移動
			pos.x -= speed_alfa * 48 * move_amount * scene_del;
			fu_speed = speed_alfa * Vec2{ -1,0 }*48 * move_amount;
			if (pos.x <= s_pos.x - 64 * move_amount)
			{
				fu_speed = speed_alfa * Vec2{ 1,0 }*48 * move_amount;
				move_switch = true;
			}
		}
	}
}

void Spike::move5()
{
	if ((pos - player_pos).length() <= 128)
	{
		double speed_alfa = 1.3;
		if (move_switch)
		{

			//下に移動

			pos.y += speed_alfa * 48 * move_amount * scene_del;

			fu_speed = speed_alfa * Vec2{ 0,1 }*48 * move_amount;

			if (pos.y >= s_pos.y + 64 * move_amount)
			{
				fu_speed = speed_alfa * Vec2{ 0,-1 }*48 * move_amount;

				move_switch = false;
			}
		}
		else
		{
			//上に移動
			pos.y -= speed_alfa * 48 * move_amount * scene_del;
			fu_speed = speed_alfa * Vec2{ 0,-1 }*48 * move_amount;
			if (pos.y <= s_pos.y - 64 * move_amount)
			{
				fu_speed = speed_alfa * Vec2{ 0,1 }*48 * move_amount;
				move_switch = true;
			}
		}

	}
}

void Spike::draw(double c_x, double c_y, bool now_map_make)
{
	Vec2 tex_pos = pos - Vec2{ c_x - 640,c_y - 480 };

	//RectF{ tex_pos,object_w,object_h }.drawFrame(2,Palette::Red);

	TextureAsset(U"Spike").draw(tex_pos);

	if (move_type > 0 && now_map_make)
	{
		FontAsset(U"sentence")(U"{}\n{}\n{}"_fmt(m_pos, move_type, move_amount)).drawAt(tex_pos + Vec2{ 32,32 });

		//通る軌跡を描く
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

void Spike::reset()
{	
	pos = s_pos;
	move_switch = false;
	hit_r = RectF{ pos,object_w,object_h };
	pre_pos = pos;
	move_timer = 0;
	speed = { 0,0 };
	fu_speed = { 0,0 };
	init_stop_time = 0.7;//最初の瞬間はブロックが動かない	

}
