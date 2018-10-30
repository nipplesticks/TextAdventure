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
		Helmet			= 0,
		Shoulders		= 1,
		Chest			= 2,
		Lederhosen		= 3, // Pants
		Gloves			= 4,
		Boots			= 5,
		Melee_Weapon	= 6,
		Ranged_Weapon	= 7,
		Shield			= 8,
		NONE			= 9
	};

	struct ItemDesc
	{
		ItemType type = Trash;
		Equippable equipType = NONE;
		int color = Color::GRAY;
	};

private:
	std::string m_name;
	Stats m_stats;
	ItemDesc m_type;
public:
	Item(ItemDesc t = {Trash, NONE, Color::GRAY});
	void setType(const ItemDesc & t);
	const ItemDesc & getType() const;

	void setName(const std::string & name);
	const std::string & getName() const;
	void setStats(const Stats & s);
	const Stats & getStats() const;
};