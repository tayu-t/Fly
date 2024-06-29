#pragma once
#include "Shot.h"
class Shot0 :
    public Shot
{
public:
	Shot0(Vec2 a_pos, int di, Shot_Master);
	Shot0(Vec2 a_pos, Vec2 s_e, Shot_Master);

	void update(double);

	void move();

    void draw(double, double);
};

