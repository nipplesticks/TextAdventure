#include "Drawable.h"
#include "../Engine/Extern.h"

Drawable::Drawable()
{
	m_spr = Sprite::NOTILE;
	m_pos = { 0,0,-10 };
	m_color = Color::BLACK;
	m_needsRedraw = true;
}

Drawable::~Drawable()
{
	m_spr = Sprite::NOTILE;
	m_pos = { 0,0,-10 };
	m_color = Color::BLACK;
}

void Drawable::setSprite(char spr)
{
	m_spr = spr;
}

char Drawable::getSprite() const
{
	return m_spr;
}

void Drawable::setColor(int color)
{
	m_color = color;
}

int Drawable::getColor() const
{
	return m_color;
}

void Drawable::setPosition(Vec pos)
{
	m_pos = pos;
}

Vec Drawable::getPosition() const
{
	return m_pos;
}

void Drawable::setRedrawState(bool state)
{
	m_needsRedraw = state;
}

bool Drawable::inNeedOfRedraw() const
{
	return m_needsRedraw;
}

void Drawable::Draw()
{
	EXT::g_drawQueue.push_back(this);
}