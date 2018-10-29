#include "Item.h"

Item::Item(Type t) : Drawable()
{
	m_name = "";
	m_type = t;
}

void Item::setType(const Type & t)
{
	m_type = t;
}

const Item::Type & Item::getType() const
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
