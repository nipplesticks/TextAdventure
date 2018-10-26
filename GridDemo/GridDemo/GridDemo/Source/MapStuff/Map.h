#pragma once
#include "../Entities/Character.h"
#include <string>
class Map
{
private:
	Vec m_sizeOfMap;
	Drawable * m_map;

public:
	Map();
	Map(const Map & other);
	~Map();

	void Loadmap(const std::string & path);

	const Vec & getSize() const;

	void Draw(std::vector<Drawable*> * dq);
private:
	void _cleanup();
	void _copy(const Map & other);
};