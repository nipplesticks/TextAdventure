#include "Inventory.h"
#include <algorithm>
Inventory::Inventory()
{
	m_sprite = nullptr;
	m_settings = { 0,0,0,0 };
	m_nrOfItems = 0;
	m_selection = -1;
	for (int i = 0; i < 9; i++)
	{
		m_isEquipped[i] = false;
	}
}

void Inventory::Init(Quad settings, const Stats & playerBaseStats)
{
	m_settings = settings;
	_alloc();
	_initText();
	_setPlayerStats(playerBaseStats, playerBaseStats.hp, playerBaseStats.hp);
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

void Inventory::UseItem(const Stats & playerBaseStats, const int & currentHP, const int & maxHP)
{
	if (m_selection != -1 && m_selection < m_nrOfItems)
	{
		_resetEverything();
		Item item = m_items[m_selection];
		_updateSelection(true);
		_removeText(item);
		Item::ItemDesc id = item.getType();

		switch (id.type)
		{
		case Item::ItemType::Equippable_item:
			
			if (m_isEquipped[id.equipType])
			{
				m_items[m_selection] = m_equippedItems[id.equipType];
				m_equippedItems[id.equipType] = item;
			}
			else
			{
				m_equippedItems[id.equipType] = item;
				for (int i = m_selection; i < m_nrOfItems - 1; i++)
				{
					_removeText(m_items[i]);
					_removeText(m_items[i + 1]);
					Vec pos = m_items[i].getPosition();
					m_items[i] = m_items[i + 1];
					m_items[i].setPosition(pos);
				}
				m_nrOfItems--;
				m_selection--;
				if (m_selection < 0) m_selection = 0;
				if (m_nrOfItems == 0) m_selection = -1;

				int c = item.getType().color;

				switch (id.equipType)
				{
				case Item::Helmet:
					m_sprite[m_avatarIndices.head].setColor(c);
					m_sprite[m_avatarIndices.head].setRedrawState(true);
					break;
				case Item::Shoulders:
					m_sprite[m_avatarIndices.lShoulder].setColor(c);
					m_sprite[m_avatarIndices.rShoulder].setColor(c);
					m_sprite[m_avatarIndices.lShoulder].setRedrawState(true);
					m_sprite[m_avatarIndices.rShoulder].setRedrawState(true);
					break;
				case Item::Chest:
					m_sprite[m_avatarIndices.chest].setColor(c);
					m_sprite[m_avatarIndices.chest].setRedrawState(true);
					break;
				case Item::Lederhosen:
					m_sprite[m_avatarIndices.pants].setColor(c);
					m_sprite[m_avatarIndices.pants].setRedrawState(true);
					break;
				case Item::Gloves:
					m_sprite[m_avatarIndices.lGlove].setColor(c);
					m_sprite[m_avatarIndices.rGlove].setColor(c);
					m_sprite[m_avatarIndices.lGlove].setRedrawState(true);
					m_sprite[m_avatarIndices.rGlove].setRedrawState(true);
					break;
				case Item::Boots:
					m_sprite[m_avatarIndices.lBoots].setColor(c);
					m_sprite[m_avatarIndices.rBoots].setColor(c);
					m_sprite[m_avatarIndices.lBoots].setRedrawState(true);
					m_sprite[m_avatarIndices.rBoots].setRedrawState(true);
					break;
				case Item::Shield:
					m_sprite[m_avatarIndices.shield].setColor(c);
					m_sprite[m_avatarIndices.shield].setRedrawState(true);
					break;
				default:
					if (id.equipType == Item::Melee_Weapon || id.equipType == Item::Ranged_Weapon)
					{
						m_sprite[m_avatarIndices.weapon].setColor(c);
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
		_updateStatsFromEquippment();
		_setPlayerStats(m_statsFromEquippment + playerBaseStats, currentHP, maxHP);
	}
}

const Stats & Inventory::getBonusStats() const
{
	return m_statsFromEquippment;
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
	for (int i = (int)Equipment.size() - 1; i >= 0; i--)
	{
		int index = 1 * m_settings.width + end - 5 - (int)Equipment.size() + i;
		m_sprite[index].setSprite(Equipment[i]);
		m_sprite[index].setColor(Color::WHITE);
	}
	std::string PlayerStats = "[Stats]";
	for (int i = (int)PlayerStats.size() - 1; i >= 0; i--)
	{
		int index = 1 * m_settings.width + middle - (int)PlayerStats.size() / 2 + i;
		m_sprite[index].setSprite(PlayerStats[i]);
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
		m_sprite[index].setRedrawState(true);
	}

	for (int i = 0; i < m_nrOfItems; i++)
	{
		std::string name = m_items[i].getName();
		std::replace(name.begin(), name.end(), '_', ' ');
		std::string type = _getStringFromType(m_items[i]);
		int length = (int)name.size();
		Vec pos = m_items[i].getPosition();

		for (int k = 0; k < length; k++)
		{
			m_sprite[pos.y * m_settings.width + k + pos.x].setSprite(name[k]);
			m_sprite[pos.y * m_settings.width + k + pos.x].setColor(m_items[i].getType().color);
		}
		int length2 = type.size();
		for (int k = 0; k < length2; k++)
		{
			m_sprite[pos.y * m_settings.width + k + pos.x + length + 1].setSprite(type[k]);
			m_sprite[pos.y * m_settings.width + k + pos.x + length + 1].setColor(m_items[i].getType().color);
		}

		Stats s = m_items[i].getStats();
		std::string firstRow = "HP: " + std::to_string(s.hp) + " AT: " + std::to_string(s.attack) + 
			" AP: " + std::to_string(s.abilityPower);
		std::string secondRow = "AR: " + std::to_string(s.armor) + " MR: " + std::to_string(s.magicArmor);

		int color = Color::LIGHT_WHITE;

		length = (int)firstRow.size();
		for (int k = 0; k < length; k++)
		{
			m_sprite[(pos.y + 1) * m_settings.width + k + pos.x + 2].setSprite(firstRow[k]);
			m_sprite[(pos.y + 1) * m_settings.width + k + pos.x + 2].setColor(color);
		}
		length = (int)secondRow.size();
		for (int k = 0; k < length; k++)
		{
			m_sprite[(pos.y + 2) * m_settings.width + k + pos.x + 2].setSprite(secondRow[k]);
			m_sprite[(pos.y + 2) * m_settings.width + k + pos.x + 2].setColor(color);
		}
	}

}

void Inventory::_createAvatar()
{
	Vec avatarPos = { m_settings.width - 15, 2,0 };
	
	// Head
	int headIndex = avatarPos.y * m_settings.width + avatarPos.x + 3;
	m_avatarIndices.head = headIndex;
	m_sprite[headIndex].setSprite('@');
	m_sprite[headIndex].setColor(Color::LIGHT_YELLOW);

	// Shoulders
	int lShoulderIndex = (avatarPos.y + 1) * m_settings.width + avatarPos.x + 2;
	int rShoudlerIndex = lShoulderIndex + 2;
	m_avatarIndices.lShoulder = lShoulderIndex;
	m_avatarIndices.rShoulder = rShoudlerIndex;

	m_sprite[lShoulderIndex].setSprite('/');
	m_sprite[lShoulderIndex].setColor(Color::LIGHT_YELLOW);
	m_sprite[rShoudlerIndex].setSprite('\\');
	m_sprite[rShoudlerIndex].setColor(Color::LIGHT_YELLOW);

	// Chest
	int ChestIndex = (avatarPos.y + 1) * m_settings.width + avatarPos.x + 3;
	m_avatarIndices.chest = ChestIndex;

	m_sprite[ChestIndex].setSprite('|');
	m_sprite[ChestIndex].setColor(Color::LIGHT_YELLOW);

	// Gloves
	int lgloveIndex = (avatarPos.y + 1) * m_settings.width + avatarPos.x + 1;
	int rgloveIndex = lgloveIndex + 4;
	m_avatarIndices.lGlove = lgloveIndex;
	m_avatarIndices.rGlove = rgloveIndex;

	m_sprite[lgloveIndex].setSprite('_');
	m_sprite[lgloveIndex].setColor(Color::LIGHT_YELLOW);
	m_sprite[rgloveIndex].setSprite('_');
	m_sprite[rgloveIndex].setColor(Color::LIGHT_YELLOW);

	// Shield
	int shieldIndex = lgloveIndex - 1;
	m_avatarIndices.shield = shieldIndex;

	m_sprite[shieldIndex].setSprite('S');
	m_sprite[shieldIndex].setColor(Color::LIGHT_YELLOW);

	// Weapon
	int weaponIndex = rgloveIndex + 1;
	m_avatarIndices.weapon = weaponIndex;

	m_sprite[weaponIndex].setSprite('W');
	m_sprite[weaponIndex].setColor(Color::LIGHT_YELLOW);

	// Pants
	int PantsIndex = (avatarPos.y + 2) * m_settings.width + avatarPos.x + 3;
	m_avatarIndices.pants = PantsIndex;

	m_sprite[PantsIndex].setSprite('A');
	m_sprite[PantsIndex].setColor(Color::LIGHT_YELLOW);

	int bootsIndex = (avatarPos.y + 3) * m_settings.width + avatarPos.x + 2;
	int RBootsIndex = bootsIndex + 2;
	m_avatarIndices.lBoots = bootsIndex;
	m_avatarIndices.rBoots = RBootsIndex;

	m_sprite[bootsIndex].setSprite('/');
	m_sprite[bootsIndex].setColor(Color::LIGHT_YELLOW);
	m_sprite[RBootsIndex].setSprite('\\');
	m_sprite[RBootsIndex].setColor(Color::LIGHT_YELLOW);



}

void Inventory::_removeText(const Item & i)
{
	std::string name = i.getName();
	int length = (int)name.size();
	Vec pos = i.getPosition();

	for (int k = 0; k < length; k++)
	{
		m_sprite[pos.y * m_settings.width + k + pos.x].setSprite(Sprite::NOTILE);
		m_sprite[pos.y * m_settings.width + k + pos.x].setRedrawState(true);
	}
}

void Inventory::_updateSelection(bool deselect)
{
	Vec pos = m_items[m_selection].getPosition();
	int length = (int)m_items[m_selection].getName().size();
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

void Inventory::_updateStatsFromEquippment()
{
	Stats s;
	for (int i = 0; i < 9; i++)
	{
		if (m_isEquipped[i])
			s = s + m_equippedItems[i].getStats();
	}
	m_statsFromEquippment = s;
}

void Inventory::_resetEverything()
{
	for (int i = 2; i < m_settings.width - 1; i++)
		for (int k = 6; k < m_settings.height - 1; k++)
		{
			m_sprite[k * m_settings.width + i].setRedrawState(true);
			m_sprite[k * m_settings.width + i].setSprite(Sprite::NOTILE);
		}
}

void Inventory::_setPlayerStats(const Stats & stats, const int & currentHP, const int & maxHP)
{
	std::string AT = "AT: " + std::to_string(stats.attack);
	std::string AP = "AP: " + std::to_string(stats.abilityPower);
	std::string AR = "AR: " + std::to_string(stats.armor);
	std::string MR = "MR: " + std::to_string(stats.magicArmor);
	std::string HP = "HP: {" + std::to_string(currentHP) + "/" + std::to_string(maxHP) + "}";


	std::string s[4] = { AT, AP, AR, MR };

	int middle = m_settings.width / 2 - 1;
	for (int i = 0; i < 4; i++)
	{
		int size = (int)s[i].size();
		for (int k = 0; k < size; k++)
		{
			int index = (i + 2) * m_settings.width + middle - (int)size / 2 + k;
			m_sprite[index].setSprite(s[i][k]);
			m_sprite[index].setColor(Color::WHITE);
		}
	}

	for (int i = (int)HP.size() - 1; i >= 0; i--)
	{
		int index = 7 * m_settings.width + m_settings.width - 5 - (int)HP.size() + i;
		m_sprite[index].setSprite(HP[i]);
		m_sprite[index].setColor(Color::WHITE);
	}

}

std::string Inventory::_getStringFromType(const Item & item)
{
	std::string returnThis = "";

	Item::ItemType it = item.getType().type;
	Item::Equippable e = item.getType().equipType;

	switch (it)
	{
	case Item::Trash:
		return "Trash";
		break;
	case Item::Usable_item:
		return "Potion";
		break;
	case Item::Equippable_item:
	{
		switch (e)
		{
		case Item::Helmet:
			return "Helmet";
			break;
		case Item::Shoulders:
			return "Shoulders";
			break;
		case Item::Chest:
			return "Chest";
			break;
		case Item::Lederhosen:
			return "Lederhosen";
			break;
		case Item::Gloves:
			return "Gloves";
			break;
		case Item::Boots:
			return "Boots";
			break;
		case Item::Melee_Weapon:
			return "M-Weapon";
			break;
		case Item::Ranged_Weapon:
			return "R-Weapon";
			break;
		case Item::Shield:
			return "Shield";
			break;
		}
		break;
	}
	}




	return returnThis;
}


