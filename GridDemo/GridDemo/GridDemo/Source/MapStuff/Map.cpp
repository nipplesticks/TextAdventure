#include "Map.h"
#include <fstream>

Map::Map()
{
	m_sizeOfMap = { 0,0,0 };
	m_map = nullptr;
}

Map::Map(const Map & other)
{
	_copy(other);
}

Map::~Map()
{
	_cleanup();
}

void Map::Loadmap(const std::string & path, Player * player)
{
	_cleanup();

	std::ifstream in;
	in.open(path);
	if (in)
	{
		in >> m_sizeOfMap.x;
		in >> m_sizeOfMap.y;

		//in >> m_nrOfRooms;

		in.ignore();

		_alloc();
		std::string line;

		for (int y = 0; y < m_sizeOfMap.y; y++)
		{
			std::getline(in, line);
			for (int x = 0; x < m_sizeOfMap.x; x++)
			{
				Vec pos = { x, y, -1 };
				int color = Color::WHITE;
				if (line[x] == Sprite::GRASS)
					color = Color::GREEN;
				else if (line[x] == Sprite::WALL)
					color = Color::PURPLE;
				else if (line[x] == Sprite::FLOOR)
					color = Color::WHITE;
				else if (line[x] == Sprite::WATER)
					color = Color::AQUA;
				else if (line[x] == Sprite::DOOR_CLOSED)
					color = Color::LIGHT_PURPLE;
				else if (line[x] == Sprite::DOOR_OPEN)
					color = Color::LIGHT_PURPLE;
				else if (line[x] == Sprite::WALL)
					color = Color::PURPLE;
				else if (line[x] == Sprite::ITEM)
				{
					Item i;
					i.setSprite(Sprite::ITEM);
					i.setPosition(Vec{ x, y, 0 });
					i.setColor(Color::YELLOW);
					m_items.push_back(i);
				}
				else if (line[x] == Sprite::PLAYER)
				{
					player->setPosition(pos);
					player->setSprite(Sprite::PLAYER);
					player->setColor(Color::RED);
				}
				m_map[y * m_sizeOfMap.x + x].setSprite(line[x]);
				m_map[y * m_sizeOfMap.x + x].setColor(color);
				m_map[y * m_sizeOfMap.x + x].setPosition(pos);
			}

		}
		_removePlayerAndItemsFromMap();

		// Load rooms and item definition

		in.close();
	}
}

void Map::InteractionMap(Character * c)
{
	if (c->isInteracting())
	{
		Vec pos = c->getPosition();
		Vec dir = c->getInteractDirection();
		Vec interactPos = pos + dir;
		if (interactPos.x >= 0 && interactPos.y >= 0 && interactPos.x < m_sizeOfMap.x && interactPos.y < m_sizeOfMap.y)
		{
			char type = m_map[interactPos.y * m_sizeOfMap.x + interactPos.x].getSprite();
			if (type == Sprite::DOOR_CLOSED)
			{
				m_map[interactPos.y * m_sizeOfMap.x + interactPos.x].setRedrawState(true);
				m_map[interactPos.y * m_sizeOfMap.x + interactPos.x].setSprite(Sprite::DOOR_OPEN);
			}
			else if (type == Sprite::DOOR_OPEN)
			{
				m_map[interactPos.y * m_sizeOfMap.x + interactPos.x].setRedrawState(true);
				m_map[interactPos.y * m_sizeOfMap.x + interactPos.x].setSprite(Sprite::DOOR_CLOSED);
			}
			else
			{
				bool found = false;
				int at = -1;
				for (int i = 0; i < m_items.size() && !found; i++)
				{
					if (interactPos.x == m_items[i].getPosition().x && interactPos.y == m_items[i].getPosition().y)
					{
						found = true;
						at = i;
					}
				}

				if (found)
				{
					Vec itemPos = m_items[at].getPosition();
					m_items.erase(m_items.begin() + at);
					m_map[itemPos.y * m_sizeOfMap.x + itemPos.x].setRedrawState(true);
				}

			}
		}
	}

	Vec pos = c->getPosition();
	Vec move = c->getMoveRequest();
	if (move.x != 0 || move.y != 0)
	{
		Vec newPos = pos + move;
		if (newPos.x >= 0 && newPos.y >= 0 && newPos.x < m_sizeOfMap.x && newPos.y < m_sizeOfMap.y)
		{
			char type = m_map[newPos.y * m_sizeOfMap.x + newPos.x].getSprite();
			bool canMove = true;
			for (auto & i : m_items)
			{
				if (newPos.x == i.getPosition().x && newPos.y == i.getPosition().y)
				{
					canMove = false;
				}
			}
			if (canMove && type != Sprite::DOOR_CLOSED && type != Sprite::NOTILE && type != Sprite::WALL && type != Sprite::WATER)
			{
				m_map[pos.y * m_sizeOfMap.x + pos.x].setRedrawState(true);
				c->MoveRequest(move);
			}
		}
	}
}

const Vec & Map::getSize() const
{
	return m_sizeOfMap;
}

void Map::Draw()
{
	for (int i = 0; i < m_sizeOfMap.x; i++)
		for (int k = 0; k < m_sizeOfMap.y; k++)
			m_map[k * m_sizeOfMap.x + i].Draw();

	for (auto & i : m_items)
		i.Draw();
}

Map & Map::operator=(const Map & other)
{
	if (this != &other)
	{
		_cleanup();
		_copy(other);
	}
	return *this;
}

void Map::_cleanup()
{
	delete[] m_map;
	m_sizeOfMap = Vec{ 0,0,0 };
}

void Map::_copy(const Map & other)
{
	m_sizeOfMap = other.m_sizeOfMap;
	_alloc();
	for (int t = 0; t < m_sizeOfMap.x * m_sizeOfMap.y; t++)
		m_map[t] = other.m_map[t];
}

void Map::_alloc()
{
	m_map = new Drawable[m_sizeOfMap.x * m_sizeOfMap.y];
	for (int i = 0; i < m_sizeOfMap.x; i++)
	{
		for (int k = 0; k < m_sizeOfMap.y; k++)
		{
			m_map[k * m_sizeOfMap.x + i] = Drawable(' ', Vec{i, k , -1}, Color::WHITE);
			m_map[k * m_sizeOfMap.x + i].setRedrawState(true);
		}
	}
}

void Map::_removePlayerAndItemsFromMap()
{
	for (int y = 0; y < m_sizeOfMap.y; y++)
	{
		for (int x = 0; x < m_sizeOfMap.x; x++)
		{
			char target = m_map[y * m_sizeOfMap.x + x].getSprite();
			if (target == Sprite::PLAYER || target == Sprite::ITEM)
			{
				char around[8] = {
					Sprite::NOTILE,
					Sprite::NOTILE,
					Sprite::NOTILE,
					Sprite::NOTILE,
					Sprite::NOTILE,
					Sprite::NOTILE,
					Sprite::NOTILE,
					Sprite::NOTILE
				};
				
				if (y > 0 && x > 0)
					around[0] = m_map[(y - 1) * m_sizeOfMap.x + (x - 1)].getSprite();
				if (y > 0)
					around[1] = m_map[(y - 1) * m_sizeOfMap.x + x].getSprite();
				if (y > 0 && x < m_sizeOfMap.x - 1)
					around[2] = m_map[(y - 1) * m_sizeOfMap.x + x + 1].getSprite();
				if (x > 0)
					around[3] = m_map[y * m_sizeOfMap.x + x -1].getSprite();
				if (x < m_sizeOfMap.x - 1)
					around[4] = m_map[y * m_sizeOfMap.x + x + 1].getSprite();
				if (x > 0 && y < m_sizeOfMap.y - 1)
					around[5] = m_map[(y + 1) * m_sizeOfMap.x + x - 1].getSprite();
				if (y < m_sizeOfMap.y - 1)
					around[6] = m_map[(y + 1) * m_sizeOfMap.x + x].getSprite();
				if (x < m_sizeOfMap.x - 1 && y < m_sizeOfMap.y - 1)
					around[7] = m_map[(y + 1) * m_sizeOfMap.x + x + 1].getSprite();

				int floor = 0;
				int grass = 0;

				for (int i = 0; i < 8; i++)
				{
					if (around[i] == Sprite::FLOOR)
						floor++;
					else if (around[i] == Sprite::GRASS)
						grass++;
				}

				if (grass > floor)
				{
					m_map[y * m_sizeOfMap.x + x].setSprite(Sprite::GRASS);
					m_map[y * m_sizeOfMap.x + x].setColor(Color::GREEN);
				}
				else
				{
					m_map[y * m_sizeOfMap.x + x].setSprite(Sprite::FLOOR);
					m_map[y * m_sizeOfMap.x + x].setColor(Color::WHITE);
				}

			}
		}
	}
}
