#pragma once

struct Vec
{
	int x, y, z;

	Vec operator+(const Vec & other) const
	{
		Vec n;
		n.x = x + other.x;
		n.y = y + other.y;
		n.z = z + other.z;
		return n;
	}
	Vec operator-(const Vec & other) const
	{
		Vec n;
		n.x = x - other.x;
		n.y = y - other.y;
		n.z = z - other.z;
		return n;
	}
	bool operator==(const Vec & other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}
	bool operator!=(const Vec & other) const
	{
		return x != other.x || y != other.y || z != other.z;
	}
};