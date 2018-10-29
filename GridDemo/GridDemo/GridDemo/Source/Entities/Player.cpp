#include "Player.h"

Player::Player() : Character()
{
	m_drawInventory = false;
}

void Player::InitInventory(Quad settings)
{
	m_inventory.Init(settings);
}

bool Player::AddItem(const Item & item)
{
	return m_inventory.AddItem(item);
}

void Player::ToggleInventoryDraw()
{
	m_drawInventory = !m_drawInventory;
	if (m_drawInventory)
	{
		m_inventory.setDrawState(true);
	}
}

bool Player::isInsideInventory()
{
	return m_drawInventory;
}

void Player::UseItem()
{
	m_inventory.UseItem();
}

void Player::setSelectionDir(const Vec & dir)
{
	m_inventory.MoveSelection(dir);
}

void Player::Draw()
{
	Drawable::Draw();
	if (m_drawInventory)
		m_inventory.Draw();
}
