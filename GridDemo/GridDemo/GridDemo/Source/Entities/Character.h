#pragma once
#include "Drawable.h"

class Character : public Drawable
{
private:
	int m_maxHP;
	int m_hp;
	Vec m_moveRequest;
	Vec m_interactDir;
public:
	Character();
	void Move(Vec dir);
	
	Vec getMoveRequest(bool reset = true);
	Vec getInteractDirection();

	void setMaxHp(int maxHP);
	int getMaxHP() const;

	void setHP(int hp);
	void addHP(int hp);
	int getHP() const;
};