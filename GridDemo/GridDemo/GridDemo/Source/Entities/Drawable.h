#pragma once
#include "../Help_Headers/Structs.h"
#include "../Help_Headers/Colors.h"
#include "../Help_Headers/Sprites.h"
#include <vector>

class Drawable
{
private:
	char m_spr;
	Vec m_pos;
	int m_color;

public:
	Drawable();
	virtual ~Drawable();

	virtual void setSprite(char spr);
	virtual char getSprite() const;

	virtual void setColor(int color);
	virtual int getColor() const;

	virtual void setPosition(Vec pos);
	virtual Vec getPosition() const;

	virtual void Draw(std::vector<Drawable*> * dq);
};