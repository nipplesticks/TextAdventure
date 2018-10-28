#pragma once
#include "Drawable.h"

class Character : public Drawable
{
private:
	int m_maxHP;
	int m_hp;
	Vec m_moveRequest;
	Vec m_interactDir;
	bool m_interactRequest;
public:
	Character();
	void MoveRequest(Vec dir);
	
	Vec getMoveRequest(bool reset = true);
	Vec getInteractDirection();
	void InteractRequest(bool req);
	bool isInteracting() const;

	void setMaxHp(int maxHP);
	int getMaxHP() const;

	void Move();

	void setHP(int hp);
	void addHP(int hp);
	int getHP() const;
};