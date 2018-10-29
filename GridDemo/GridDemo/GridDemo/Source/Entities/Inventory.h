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
	// TODO :: PUT ALL EQUIPPED STATS HERE!!!!
	 

	int m_nrOfItems;
	int m_selection;
	Quad m_settings; // view space;
	AvatarIndices m_avatarIndices;

public:
	Inventory();
	void Init(Quad settings);
	void Draw();
	void setDrawState(bool state);
	bool AddItem(const Item & item);
	void MoveSelection(const Vec & dir);
	Stats UseItem();

private:
	void _alloc();
	void _setItemPos();
	void _cleanup();
	void _initText();
	void _updateText();
	void _createAvatar();
	void _updateSelection(bool deselect = false);
};