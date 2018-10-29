#include "Item.h"

Item::Item(ItemDesc t) : Drawable()
{
	m_name = "";
	m_type = t;
}

void Item::setType(const ItemDesc & t)
{
	m_type = t;
}

const Item::ItemDesc & Item::getType() const
{
	return m_type;
}

void Item::setName(const std::string & name)
{
	m_name = name;
}

const std::string & Item::getName() const
{
	return m_name;
}

void Item::setStats(const Stats & s)
{
	m_stats = s;
}

const Stats & Item::getStats() const
{
	return m_stats;
}
