#include "Character.h"

Character::Character() : Drawable()
{
	m_stats.hp = 10;
	m_stats.attack = 1;
	m_stats.abilityPower = 1;
	m_stats.armor = 1;
	m_stats.magicArmor = 1;

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

void Character::Move()
{
	if (m_moveRequest.x != 0 || m_moveRequest.y != 0)
	{
		setPosition(getPosition() + getMoveRequest());
		setRedrawState(true);
	}
}

void Character::setStats(const Stats & s)
{
	m_stats = s;
}

const Stats & Character::getStats() const
{
	return m_stats;
}

void Character::addStats(const Stats & s)
{
	m_stats = m_stats + s;
}