#pragma once
#include "Drawable.h"

class Character : public Drawable
{
private:
	Stats m_stats;
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

	void Move();

	void setStats(const Stats & s);
	const Stats &  getStats() const;
	void addStats(const Stats & s);
};