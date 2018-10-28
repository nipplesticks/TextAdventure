#include "Inventory.h"

Inventory::Inventory()
{
	m_sprite = nullptr;
	m_settings = { 0,0,0,0 };
}

void Inventory::Init(Quad settings)
{
	m_settings = settings;
	_alloc();
}

void Inventory::Draw()
{
	for (int x = 0; x < m_settings.width; x++)
		for (int y = 0; y < m_settings.height; y++)
		{
			m_sprite->Draw();
		}
}

void Inventory::_alloc()
{
	m_sprite = new Drawable[m_settings.width * m_settings.height];
	for (int i = 0; i < m_settings.width; i++)
		for (int k = 0; k < m_settings.height; k++)
		{
			char spr = Sprite::NOTILE;
			int c = Color::GRAY;
			if (i == 0 || i == m_settings.width - 1 || k == 0 || k == m_settings.height)
			{
				spr = Sprite::BORDER;
				c = Color::LIGHT_YELLOW;
			}
			m_sprite[k * m_settings.width + i] = Drawable(spr, Vec{ i, k, 10 }, c);
		}
}

void Inventory::_cleanup()
{
	delete[] m_sprite;
	m_settings = { 0,0,0,0 };
}
