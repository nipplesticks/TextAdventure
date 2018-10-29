#include "Character.h"

Character::Character() : Drawable()
{
	m_maxHP = 10;
	m_stats.hp = m_maxHP;
	m_stats.attack = 1;
	m_stats.magicAttack = 1;
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

void Character::setMaxHp(int maxHP)
{
	m_maxHP = maxHP;
	if (m_stats.hp > m_maxHP)
		m_stats.hp = m_maxHP;
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

void Character::setHP(int hp)
{
	if (hp > m_maxHP)
		m_stats.hp = m_maxHP;
	else
		m_stats.hp = hp;
}

void Character::addHP(int hp)
{
	m_stats.hp += hp;
	if (m_stats.hp > m_maxHP)
		m_stats.hp = m_maxHP;
	else if (m_stats.hp < 0)
		m_stats.hp = 0;
}

int Character::getHP() const
{
	return m_stats.hp;
}
