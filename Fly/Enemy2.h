#pragma once
#include "Enemy.h"
class Enemy2 :
    public Enemy
{
public:
	Enemy2(int, int);

	void update(double, Vec2);

	void update_live();

	void move();

	void draw(double, double);

	void reset();//リトライ時に使う
};

