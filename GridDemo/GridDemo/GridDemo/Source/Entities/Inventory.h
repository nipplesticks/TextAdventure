#pragma once
#include "Drawable.h"
#include "Item.h"
#include <string>

class Inventory
{
private:
	struct AvatarIndices
	{
		int head,
			lShoulder, 
			rShoulder, 
			chest, 
			shield,
			lGlove, 
			rGlove, 
			weapon, 
			pants, 
			lBoots, 
			rBoots;
	};
private:
	static const int MAX_ITEMS = 16;
	Drawable * m_sprite;
	Item m_items[MAX_ITEMS];
	Item m_equippedItems[9];
	bool m_isEquipped[9];
	Stats m_statsFromEquippment;
	 

	int m_nrOfItems;
	int m_selection;
	Quad m_settings; // view space;
	AvatarIndices m_avatarIndices;

public:
	Inventory();
	void Init(Quad settings, const Stats & playerBaseStats);
	void Draw();
	void setDrawState(bool state);
	bool AddItem(const Item & item);
	void MoveSelection(const Vec & dir);
	void UseItem(const Stats & playerBaseStats, const int & currentHP, const int & maxHP);
	const Stats & getBonusStats() const;

private:
	void _alloc();
	void _setItemPos();
	void _cleanup();
	void _initText();
	void _updateText();
	void _createAvatar();
	void _removeText(const Item & i);
	void _updateSelection(bool deselect = false);
	void _updateStatsFromEquippment();
	void _resetEverything();
	void _setPlayerStats(const Stats & stats, const int & currentHP, const int & maxHP);
	std::string _getStringFromType(const Item & item);
};