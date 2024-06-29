#include "stdafx.h"
#include "Enemy.h"


void Enemy::update_die()
{
	//死亡時のエフェクトが動いているかどうか
	bool death_effect_update = false;
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

	

	if (death_effect_update == false && shots.size() == 0)
	{
		exit = false;
	}
}


void Enemy::death()
{
	live = false;
	effects << Game_Effect(pos + Vec2{ e_w / 2,e_h / 2 }, Effect_Kind::death);
}

void Enemy::damage(int dame)
{
	HP -= dame;
	if (HP <= 0)
	{
		death();
	}
}


