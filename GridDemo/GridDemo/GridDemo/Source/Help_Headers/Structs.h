#pragma once

struct Stats
{
	int hp = 0;
	int attack = 0;
	int magicAttack = 0;

	int armor = 0;
	int magicArmor = 0;
	Stats operator+(const Stats & other)
	{
		Stats s;

		s.hp =			hp			+ other.hp;
		s.attack =		attack		+ other.attack;
		s.armor =		armor		+ other.armor;
		s.magicAttack = magicAttack + other.magicAttack;
		s.magicArmor = magicArmor + other.magicArmor;
		return s;
	}
	Stats operator-(const Stats & other)
	{
		Stats s;

		s.hp =			hp			- other.hp;
		s.attack =		attack		- other.attack;
		s.armor =		armor		- other.armor;
		s.magicAttack = magicAttack - other.magicAttack;
		s.magicArmor = magicArmor - other.magicArmor;
		return s;
	}
};

struct Vec
{
	int x, y, z;

	Vec operator+(const Vec & other) const
	{
		Vec n;
		n.x = x + other.x;
		n.y = y + other.y;
		n.z = z + other.z;
		return n;
	}
	Vec operator-(const Vec & other) const
	{
		Vec n;
		n.x = x - other.x;
		n.y = y - other.y;
		n.z = z - other.z;
		return n;
	}
	bool operator==(const Vec & other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}
	bool operator!=(const Vec & other) const
	{
		return x != other.x || y != other.y || z != other.z;
	}
};

struct Quad
{
	int top, left, width, height;
};