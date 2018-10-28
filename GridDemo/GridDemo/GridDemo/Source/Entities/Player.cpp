#include "Player.h"

Player::Player() : Character()
{
	m_drawInventory = false;
}

void Player::InitInventory(Quad settings)
{
	m_inventory.Init(settings);
}

void Player::ToggleInventoryDraw()
{
	m_drawInventory = !m_drawInventory;
}

void Player::Draw()
{
	Drawable::Draw();
	if (m_drawInventory)
		m_inventory.Draw();
}
