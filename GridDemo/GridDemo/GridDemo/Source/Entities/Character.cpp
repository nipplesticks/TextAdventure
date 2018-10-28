#include "Character.h"

Character::Character() : Drawable()
{
	m_maxHP = 100;
	m_hp = m_maxHP;
	m_moveRequest = { 0,0,0 };
	m_interactDir = { 0,0,0 };
	m_interactRequest = false;
}

void Character::MoveRequest(Vec dir)
{
	m_interactDir = dir;
	m_moveRequest = dir;
}

Vec Character::getMoveRequest(bool reset)
{
	Vec dir = m_moveRequest;
	if (reset)
		m_moveRequest = { 0,0,0 };

	return dir;
}

Vec Character::getInteractDirection()
{
	return m_interactDir;
}

void Character::InteractRequest(bool req)
{
	m_interactRequest = req;
}

bool Character::isInteracting() const
{
	return m_interactRequest;
}

void Character::setMaxHp(int maxHP)
{
	m_maxHP = maxHP;
	if (m_hp > m_maxHP)
		m_hp = m_maxHP;
}

int Character::getMaxHP() const
{
	return m_maxHP;
}

void Character::Move()
{
	if (m_moveRequest.x != 0 || m_moveRequest.y != 0)
	{
		setPosition(getPosition() + getMoveRequest());
		setRedrawState(true);
	}
}

void Character::setHP(int hp)
{
	if (hp > m_maxHP)
		m_hp = m_maxHP;
	else
		m_hp = hp;
}

void Character::addHP(int hp)
{
	m_hp += hp;
	if (m_hp > m_maxHP)
		m_hp = m_maxHP;
	else if (m_hp < 0)
		m_hp = 0;
}

int Character::getHP() const
{
	return m_hp;
}
