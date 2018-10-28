#pragma once
#include "Drawable.h"

class Inventory
{
private:
	Drawable * m_sprite;
	Quad m_settings; // view space;

public:
	Inventory();
	void Init(Quad settings);
	void Draw();
private:
	void _alloc();
	void _cleanup();
};