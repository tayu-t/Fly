#pragma once
#include "Enemy.h"


class Enemy0 :
    public Enemy
{
public:
	Enemy0(int, int);

	void update(double,Vec2);

	void update_live();

	void move();

	void draw(double,double);

	void reset();//リトライ時に使う
};

