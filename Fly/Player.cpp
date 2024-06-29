#include "stdafx.h"
#include "Player.h"
#include"Play.h"

void Player::init(int m_x,int m_y,int hp)
{
	spon_x = m_x;
	spon_y = m_y;

	shots.clear();
	effects.clear();

	dire = 1;

	have_star = 0;

	normal_HP_max = hp;



	HP = normal_HP_max;
	HP_max = normal_HP_max;

	start_pos = Vec2{ spon_x,spon_y }*64 + Vec2{32-p_w/2,32-p_h/2};//texture(四角形)の左上の座標
	pos = start_pos;//textureの左上
	pre_pos = pos;//1フレーム前のpos

	p_w = 48;
	p_h = 48;

	wait_click = 0.3;

	speed = { 0,0 };//playerの速度
}

void Player::update(double scene_deltatime)
{
	scene_del = scene_deltatime;

	//エフェクトの更新
	for (auto& e : effects)
	{
		e.update(scene_del);
	}
	for (auto& shot : shots)
	{
		//弾の移動
		shot->update(scene_del);
	}

	switch (live)
	{
	case true:
		update_live();
		break;
	case false:
		update_die();
		break;
	}

	effects.remove_if([&](Game_Effect effect) {return effect.get_exit() == false; });
	shots.remove_if([&](std::shared_ptr<Shot> shot) {return shot->get_exit() == false; });
}

void Player::update_live()
{

	move();//移動
	attack();


	muteki();

	if (grounded)
	{
		tex_index = 4;
	}
	else
	{
		tex_index_timer += scene_del;

		

		if (tex_index_timer >= (speed.y < 10 ? tex_index_span*0.5 : tex_index_span))
		{
			tex_index_timer = 0;

			if (tex_bool)
			{
				tex_index++;
				if (tex_index == 5)
				{
					tex_bool = false;
					tex_index = 4;
				}
			}
			else
			{
				tex_index--;
				if (tex_index == 0)
				{
					tex_bool = true;
					tex_index = 1;
				}
			}

		}
	}


	//反射板のupdate
	//reflecter.update(scene_del, pos + Vec2{dire * 35 ,0 });


	hit_r = RectF{ pos,p_w,p_h };


}


void Player::attack()
{
	//弾を出す
	shot_timer += scene_del;

	if (shot_timer >= shot_span)
	{
		if (KeySpace.pressed() && wait_click <= 0)
		{
			shots << std::make_shared<Shot0>(pos + Vec2{p_w/2,p_h/2} + Vec2{ 20 * dire,0 }, dire, Shot_Master::player);
			shot_timer = 0;

			if (true)
			{
		
				if (abs(speed.x) <= abs(200))
				{
					speed.x += -dire * 25;//押し返される
				}
			}
		}
	}
}



void Player::move()
{
	//ボタンを押したときのスピード（注）左上に飛ぶときはx成分は-200になる
	Vec2 push_speed{ 125,-250 };

	bool push_fly = false;//飛ぶボタンを押したか

	if (wait_click > 0)
	{
		wait_click -= scene_del;
	}
	else
	{


		if (KeyRight.down())
		{
			push_fly = true;
			//grounded = false;
			speed = push_speed;//右上に飛ぶ
			dire = 1;
		}
		if (KeyLeft.down())
		{

			push_fly = true;
			//grounded = false;
			speed = Vec2{ -push_speed.x,push_speed.y };//左上に飛ぶ
			dire = -1;
		}
	}



	//重力
	if (speed.y <= 200)
	{
		speed.y += gravity * scene_del;
	}

	pre_pos = pos;//1フレーム前のposの更新
	pos += speed * scene_del;//posの更新

	if (grounded)
	{
		pos += ground_speed * scene_del;

		if (push_fly == false)
		{


			speed.x *= 0.85;

			if (abs(speed.x) < 1) { speed.x = 0; }
		}
	}

}

void Player::update_die()
{
	bool death_effect_update = false;//trueなら動いている
	for (auto& effect : effects)
	{
		if (effect.get_be_wait_effect())
		{
			if (effect.get_exit())
			{
				death_effect_update = true;
			}
		}
	}

	//死亡エフェクトが終わったなら生き返る
	if (death_effect_update == false)
	{
		can_restart = true;
	}
}

void Player::draw(double c_x,double c_y)
{
	this->c_x = c_x;
	this->c_y = c_y;

	tex_pos = pos  - Vec2{ c_x - 640,c_y - 480 };

	//当たり判定の表示
	if (live)
	{

		draw_player();

		//HPの表示
		for (int i = 0; i < HP_max; i++)
		{
			TextureAsset(U"heartframe").scaled(1.5).draw(200 + 60 * i, 0);
		}

		for (int i = 0; i < HP; i++)
		{
			TextureAsset(U"heart").scaled(1.5).draw(200 + 60 * i, 0);
		}

	
	}

	//取得した星の数
	Shape2D::Star(25,Vec2{ 25,65 }).draw(Palette::Yellow);
	FontAsset(U"menu")(U"×{}"_fmt(have_star)).drawAt(75, 70, Palette::Black);


	//弾の描画
	for (auto& shot : shots)
	{
		shot->draw(c_x,c_y);
	}

	//effectの描画
	for (auto& e : effects)
	{
		e.draw(c_x,c_y);
	}

	draw_debug();

}

void Player::draw_player()
{
	if (muteki_timer > 0)
	{

		//RectF(hit_r.pos - Vec2{c_x-640,c_y-480}, p_w, p_h).draw(Palette::Red);
		if (dire == 1)
		{
			//右
			TextureAsset(U"Bard_r_{}"_fmt(tex_index)).draw(tex_pos,Palette::Red);
		}
		else
		{
			//左
			TextureAsset(U"Bard_l_{}"_fmt(tex_index)).draw(tex_pos, Palette::Red);
		}
	}
	else
	{

		//RectF(hit_r.pos - Vec2{ c_x - 640,c_y - 480 }, p_w, p_h).draw(Palette::Black);

		if (dire == 1)
		{
			//右
			TextureAsset(U"Bard_r_{}"_fmt(tex_index)).draw(tex_pos);
		}
		else
		{
			//左
			TextureAsset(U"Bard_l_{}"_fmt(tex_index)).draw(tex_pos);
		}

	}
}

void Player::draw_debug()
{
}

void Player::death(int r_x,int r_y)
{
	spon_x = r_x;
	spon_y = r_y;

	live = false;
	can_restart = false;

	effects << Game_Effect(pos + Vec2{p_w/2,p_h/2},Effect_Kind::death);
}

void Player::reset()
{
	wait_click = 0.3;
	have_star = 0;



	HP = normal_HP_max;
	HP_max = normal_HP_max;

	HP = HP_max;
	start_pos = Vec2{ spon_x,spon_y }*64 + Vec2{ 32-p_w/2,32-p_h/2 };
	pos = start_pos;
	pre_pos = pos;//1フレーム前のpos
	speed = { 0,0 };//playerの速度
	hit_r = RectF{ pos,p_w,p_h };
	live = true;
	shot_timer = 1;
	shots.clear();
	effects.clear();
	muteki_timer = 0;
}

void Player::damage(int dama)
{
	if (muteki_timer <= 0)
	{

		
		HP -= dama;
		muteki_timer = muteki_time;
	}
}

void Player::muteki()
{
	if (muteki_timer > 0)
	{
		muteki_timer -= scene_del;
	}
}

void Player::init_get_star()
{
	//星の所持数を増やす
	have_star++;
	effects << Game_Effect(pos - Vec2{c_x-640,c_y-480}, Effect_Kind::get_star);
}



