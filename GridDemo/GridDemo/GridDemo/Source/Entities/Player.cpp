#include "Player.h"

Player::Player() : Character()
{
	m_drawInventory = false;
	m_currentHP = getStats().hp;
}

void Player::InitInventory(Quad settings)
{
	m_inventory.Init(settings, getStats());
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
	m_inventory.UseItem(getStats(), m_currentHP);
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

void Player::setHP(int hp)
{
	m_currentHP = hp;
	if (hp > getStats().hp + m_inventory.getBonusStats().hp)
	{
		m_currentHP = getStats().hp + m_inventory.getBonusStats().hp;
	}
	else if (hp < 0)
		m_currentHP = 0;
}

void Player::AddHP(int hp)
{
	setHP(m_currentHP + hp);
}

int Player::getHP() const
{
	return m_currentHP;
}
