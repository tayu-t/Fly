#include "stdafx.h"
#include "Empty.h"

Empty::Empty(int m_x,int m_y)
{
	can_hit = false;

	change = false;

	m_pos = Vec2{ m_x,m_y };
	pos = Vec2{ m_x,m_y }*Vec2{ 64,64 };
	pre_pos = pos;
	s_pos = pos;

	object_kind = Object_Kind_Empty;
	first_object_kind = object_kind;
	
}


void Empty::draw(double c_x, double c_y, bool no_use)
{
	if (change)
	{
		Vec2 tex_pos = pos - Vec2{ c_x - 640,c_y - 480 };

		RectF{ tex_pos,object_w,object_h }.draw(Palette::Brown);
	}
}

void Empty::reset()
{
	change = false;
	can_hit = false;
}
