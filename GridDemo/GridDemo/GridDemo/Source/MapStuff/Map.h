#pragma once
#include "../Entities/Player.h"
#include <string>
class Map
{
private:
	Vec m_sizeOfMap;
	Drawable * m_map;

	/*int m_nrOfRooms;
	Drawable ** m_rooms;*/
public:
	Map();
	Map(const Map & other);
	~Map();

	void Loadmap(const std::string & path, Player * player);
	void Move(Character * c);

	const Vec & getSize() const;

	void Draw();

	Map & operator=(const Map & other);
private:
	void _cleanup();
	void _copy(const Map & other);
	void _alloc();
	void _removePlayerAndItemsFromMap();
};