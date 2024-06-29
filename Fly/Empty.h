#pragma once
#include "Map_Object.h"

class Empty :
	public Map_Object
{
private:


public:
	Empty() {};
	Empty(int,int);

	//bool created_block();
	void reset();
	void draw(double c_x, double c_y, bool now_map_make);
};

