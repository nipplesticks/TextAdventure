#pragma once

#include "Drawable.h"
#include <string>

class Item : public Drawable
{
public:
	enum ItemType
	{
		Trash = 0,
		Usable_item = 1,
		Equippable_item = 2
	};

	enum Equippable
	{
		NONE = 0,
		Helmet = 1,
		Shoulders = 2,
		Chest = 3,
		Waist = 4,
		Lederhosen = 5, // Pants
		Gloves = 6,
		Boots = 7,
		Melee_Weapon = 8,
		Ranged_Weapon = 9,
		Shield = 10
	};

	struct Type
	{
		ItemType type = Trash;
		Equippable equipType = NONE;
		int color = Color::GRAY;
	};

private:
	std::string m_name;
	Stats m_stats;
	Type m_type;
public:
	Item(Type t = {Trash, NONE});
	void setType(const Type & t);
	const Type & getType() const;

	void setName(const std::string & name);
	const std::string & getName() const;
	void setStats(const Stats & s);
	const Stats & getStats() const;
};