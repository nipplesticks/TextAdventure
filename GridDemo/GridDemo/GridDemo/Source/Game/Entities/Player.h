#pragma once
#include "Character.h"
#include "Inventory.h"

class Player : public Character
{
private:
	Inventory m_inventory;
	int m_currentHP;
	bool m_drawInventory;

public:
	Player();
	void InitInventory(Quad settings);
	bool AddItem(const Item & item);
	void ToggleInventoryDraw();
	bool isInsideInventory();
	void UseItem();
	void setSelectionDir(const Vec & dir);
	void Draw() override;
	void setHP(int hp);
	void AddHP(int hp);
	int getHP() const;
};