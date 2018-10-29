#include "Inventory.h"

Inventory::Inventory()
{
	m_sprite = nullptr;
	m_settings = { 0,0,0,0 };
	m_nrOfItems = 0;
	m_selection = -1;
	for (int i = 0; i < 8; i++)
	{
		m_isEquipped[i] = false;
	}
}

void Inventory::Init(Quad settings)
{
	m_settings = settings;
	_alloc();
	_initText();
	_setItemPos();
}

void Inventory::Draw()
{
	for (int x = 0; x < m_settings.width; x++)
		for (int y = 0; y < m_settings.height; y++)
		{
			m_sprite[y * m_settings.width + x].DrawAsHud();
		}
}

void Inventory::setDrawState(bool state)
{
	for (int i = 0; i < m_settings.width * m_settings.height; i++)
		m_sprite[i].setRedrawState(state);
	for (int i = 0; i < m_nrOfItems; i++)
		m_items[i].setRedrawState(state);
}

bool Inventory::AddItem(const Item & item)
{
	bool added = false;
	if (m_nrOfItems < MAX_ITEMS)
	{
		if (m_selection != -1)
			_updateSelection(true);
		Vec pos = m_items[m_nrOfItems].getPosition();
		m_selection = m_nrOfItems;
		m_items[m_nrOfItems] = item;
		m_items[m_nrOfItems++].setPosition(pos);
		added = true;
		_updateText();
		_updateSelection();
	}
	return added;
}

void Inventory::MoveSelection(const Vec & dir)
{
	if (m_nrOfItems != 0)
	{
		_updateSelection(true);

		int d = 1;
		if (dir.x < 0)
			d *= -1;

		int lastSel = m_selection;
		m_selection += (MAX_ITEMS / 2) * dir.x;
		m_selection += dir.y;
		if (m_selection >= m_nrOfItems)
			m_selection = lastSel;
		else if (m_selection < 0)
			m_selection = lastSel;
		_updateSelection();
	}
}

Stats Inventory::UseItem()
{
	Stats s;
	if (m_selection != -1 && m_selection < m_nrOfItems)
	{
		_updateSelection(false);
		Item i = m_items[m_selection];
		s = i.getStats();
		Item::ItemDesc id = i.getType();

		switch (id.type)
		{
		case Item::ItemType::Equippable_item:
			
			if (m_isEquipped[id.equipType])
			{
				m_items[m_selection] = m_equippedItems[id.equipType];
				m_equippedItems[id.equipType] = i;
			}
			else
			{
				m_equippedItems[id.equipType] = i;
				for (int i = m_selection; i < m_nrOfItems; i++)
				{
					m_items[i] = m_items[i + 1];
				}
				m_nrOfItems--;
				m_selection--;
				if (m_selection < 0) m_selection = 0;
				if (m_nrOfItems == 0) m_selection = -1;


				switch (id.equipType)
				{
				case Item::Helmet:
					m_sprite[m_avatarIndices.head].setColor(Color::LIGHT_RED);
					break;
				case Item::Shoulders:
					m_sprite[m_avatarIndices.lShoulder].setColor(Color::LIGHT_RED);
					m_sprite[m_avatarIndices.rShoulder].setColor(Color::LIGHT_RED);
					break;
				case Item::Chest:
					m_sprite[m_avatarIndices.chest].setColor(Color::LIGHT_RED);
					break;
				case Item::Waist:
					//m_sprite[m_avatarIndices.lShoulder].setColor(Color::LIGHT_RED);
					break;
				case Item::Lederhosen:
					m_sprite[m_avatarIndices.pants].setColor(Color::LIGHT_RED);
					break;
				case Item::Gloves:
					m_sprite[m_avatarIndices.lGlove].setColor(Color::LIGHT_RED);
					m_sprite[m_avatarIndices.rGlove].setColor(Color::LIGHT_RED);
					break;
				case Item::Boots:
					m_sprite[m_avatarIndices.lBoots].setColor(Color::LIGHT_RED);
					m_sprite[m_avatarIndices.rBoots].setColor(Color::LIGHT_RED);
					break;
				case Item::Shield:
					break;
				default:
					if (id.equipType == Item::Melee_Weapon || id.equipType == Item::Ranged_Weapon)
					{
						m_sprite[m_avatarIndices.weapon].setColor(Color::LIGHT_RED);
						m_sprite[m_avatarIndices.weapon].setRedrawState(true);
					}
					break;
				}
			}
			break;
		}


		if (m_selection != -1)
			_updateSelection();
		_updateText();
	}
	return s;
}

void Inventory::_alloc()
{
	m_sprite = new Drawable[m_settings.width * m_settings.height];
	for (int i = 0; i < m_settings.width; i++)
		for (int k = 0; k < m_settings.height; k++)
		{
			char spr = Sprite::NOTILE;
			int c = Color::GRAY;
			if (i == 0 || i == m_settings.width - 1 || k == 0 || k == m_settings.height - 1)
			{
				spr = Sprite::BORDER;
				c = Color::LIGHT_YELLOW;
			}
			m_sprite[k * m_settings.width + i] = Drawable(spr, Vec{ m_settings.left + i, m_settings.top+ k, 10 }, c);
		}
	_createAvatar();
}

void Inventory::_setItemPos()
{
	int startX = 2;
	int startY = 6;
	int offY = 0;
	for (int i = 0; i < MAX_ITEMS; i++)
	{
		if (i != 0 && i % 8 == 0)
		{
			startX += m_settings.width / 2;
			offY = 0;
		}

		m_items[i].setPosition(Vec{startX, startY + offY });
		offY += 3;
	}
}

void Inventory::_cleanup()
{
	delete[] m_sprite;
	m_settings = { 0,0,0,0 };
}

void Inventory::_initText()
{
	std::string inventory = "[INVENTORY]";

	int start = 7;
	int end = m_settings.width - 1;
	int middle = (end) / 2 + 1;
	// InventoryText
	for (int i = 0; i < inventory.size(); i++)
	{
		int index = 1 * m_settings.width + start + i;
		m_sprite[index].setSprite(inventory[i]);
		m_sprite[index].setColor(Color::WHITE);
	}
	std::string Equipment = "[Equipped]";
	for (int i = inventory.size() - 1; i >= 0; i--)
	{
		int index = 1 * m_settings.width + end - 5 - inventory.size() + i;
		m_sprite[index].setSprite(Equipment[i]);
		m_sprite[index].setColor(Color::WHITE);
	}


	_updateText();
}

void Inventory::_updateText()
{
	std::string capacity = "Capacity: " + std::to_string(m_nrOfItems) + "/" + std::to_string(MAX_ITEMS);
	int start =  6;
	int end = m_settings.width - 1;
	int middle = (end) / 2 + 1;
	for (int i = 0; i < capacity.size(); i++)
	{
		int index = 2 * m_settings.width + start + i;
		m_sprite[index].setSprite(capacity[i]);
		m_sprite[index].setColor(Color::WHITE);
	}

	for (int i = 0; i < m_nrOfItems; i++)
	{
		std::string name = m_items[i].getName();
		int length = name.size();
		Vec pos = m_items[i].getPosition();

		for (int k = 0; k < length; k++)
		{
			m_sprite[pos.y * m_settings.width + k + pos.x].setSprite(name[k]);
			m_sprite[pos.y * m_settings.width + k + pos.x].setColor(m_items[i].getType().color);
		}
	}

}

void Inventory::_createAvatar()
{
	Vec avatarPos = { m_settings.width - 15, 2,0 };
	
	// Head
	int headIndex = avatarPos.y * m_settings.width + avatarPos.x + 2;
	m_avatarIndices.head = headIndex;
	m_sprite[headIndex].setSprite('@');
	m_sprite[headIndex].setColor(Color::LIGHT_YELLOW);

	// Shoulders
	int lShoulderIndex = (avatarPos.y + 1) * m_settings.width + avatarPos.x + 1;
	int rShoudlerIndex = lShoulderIndex + 2;
	m_avatarIndices.lShoulder = lShoulderIndex;
	m_avatarIndices.rShoulder = rShoudlerIndex;

	m_sprite[lShoulderIndex].setSprite('/');
	m_sprite[lShoulderIndex].setColor(Color::LIGHT_YELLOW);
	m_sprite[rShoudlerIndex].setSprite('\\');
	m_sprite[rShoudlerIndex].setColor(Color::LIGHT_YELLOW);

	// Chest
	int ChestIndex = (avatarPos.y + 1) * m_settings.width + avatarPos.x + 2;
	m_avatarIndices.chest = ChestIndex;

	m_sprite[ChestIndex].setSprite('|');
	m_sprite[ChestIndex].setColor(Color::LIGHT_YELLOW);

	// Gloves
	int lgloveIndex = (avatarPos.y + 1) * m_settings.width + avatarPos.x;
	int rgloveIndex = lgloveIndex + 4;
	m_avatarIndices.lGlove = lgloveIndex;
	m_avatarIndices.rGlove = rgloveIndex;

	m_sprite[lgloveIndex].setSprite('_');
	m_sprite[lgloveIndex].setColor(Color::LIGHT_YELLOW);
	m_sprite[rgloveIndex].setSprite('_');
	m_sprite[rgloveIndex].setColor(Color::LIGHT_YELLOW);

	// Weapon
	int weaponIndex = rgloveIndex + 1;
	m_avatarIndices.weapon = weaponIndex;

	m_sprite[weaponIndex].setSprite('W');
	m_sprite[weaponIndex].setColor(Color::LIGHT_YELLOW);

	// Pants
	int PantsIndex = (avatarPos.y + 2) * m_settings.width + avatarPos.x + 2;
	m_avatarIndices.pants = PantsIndex;

	m_sprite[PantsIndex].setSprite('A');
	m_sprite[PantsIndex].setColor(Color::LIGHT_YELLOW);

	int bootsIndex = (avatarPos.y + 3) * m_settings.width + avatarPos.x + 1;
	int RBootsIndex = bootsIndex + 2;
	m_avatarIndices.lBoots = bootsIndex;
	m_avatarIndices.rBoots = RBootsIndex;

	m_sprite[bootsIndex].setSprite('/');
	m_sprite[bootsIndex].setColor(Color::LIGHT_YELLOW);
	m_sprite[RBootsIndex].setSprite('\\');
	m_sprite[RBootsIndex].setColor(Color::LIGHT_YELLOW);



}

void Inventory::_updateSelection(bool deselect)
{
	Vec pos = m_items[m_selection].getPosition();
	int length = m_items[m_selection].getName().size();
	if (!deselect)
	{
		m_sprite[pos.y * m_settings.width + pos.x - 1].setSprite('[');
		m_sprite[pos.y * m_settings.width + pos.x - 1].setRedrawState(true);
		m_sprite[pos.y * m_settings.width + pos.x + length].setSprite(']');
		m_sprite[pos.y * m_settings.width + pos.x + length].setRedrawState(true);
	}
	else
	{
		m_sprite[pos.y * m_settings.width + pos.x - 1].setSprite(Sprite::NOTILE);
		m_sprite[pos.y * m_settings.width + pos.x - 1].setRedrawState(true);
		m_sprite[pos.y * m_settings.width + pos.x + length].setSprite(Sprite::NOTILE);
		m_sprite[pos.y * m_settings.width + pos.x + length].setRedrawState(true);
	}

}

