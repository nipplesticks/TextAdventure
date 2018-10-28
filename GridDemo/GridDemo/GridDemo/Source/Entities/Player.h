#pragma once
#include "Character.h"
#include "Inventory.h"

class Player : public Character
{
private:
	Inventory m_inventory;
	bool m_drawInventory;
public:
	Player();
	void InitInventory(Quad settings);
	void ToggleInventoryDraw();
	void Draw() override;
};