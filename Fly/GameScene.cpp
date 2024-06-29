#include "stdafx.h"
#include "GameScene.h"

void GameScene::Shake_counter()
{
	double min = -2 * shake_counter;
	double max = 2 * shake_counter;

	shake = Random(min, max);

	shake_counter *= 0.85;

	if (shake_counter <= 0.1)
	{
		shake_counter = 0;
	}
}
