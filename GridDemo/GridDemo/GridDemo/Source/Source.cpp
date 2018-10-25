#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Timer.h"
#include <stdlib.h>
#include "rlutil.h"

const int BLACK = 0;
const int BLUE = 1;
const int GREEN = 2;
const int AQUA = 3;
const int RED = 4;
const int PURPLE = 5;
const int YELLOW = 6;
const int WHITE = 7;
const int GRAY = 8;
const int LIGHT_BLUE = 9;
const int LIGHT_GREEN = 10;
const int LIGHT_AQUA = 11;
const int LIGHT_RED = 12;
const int LIGHT_PURPLE = 13;
const int LIGHT_YELLOW = 14;
const int LIGHT_WHITE = 15;

struct Vec
{
	int x, y, z;

	Vec operator+(const Vec & other)
	{
		Vec n;
		n.x = x + other.x;
		n.y = y + other.y;
		n.z = z + other.z;
		return n;
	}

};
struct Drawable
{
	char spr = ' ';
	Vec pos = { 0,0,0 };
	int color = 7;
};
struct Map
{
	Vec mapSize = { 0,0,0 };
	Drawable * map = nullptr;

	void alloc()
	{
		map = new Drawable[mapSize.x * mapSize.y];
		for (int i = 0; i < mapSize.x; i++)
		{
			for (int k = 0; k < mapSize.y; k++)
			{
				map[k * mapSize.x + i] = { ' ', i, k, -1, 7};
			}
		}
	}

	void Draw(std::vector<Drawable> * drawQueue)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			for (int k = 0; k < mapSize.y; k++)
			{
				drawQueue->push_back(map[k * mapSize.x + i]);
			}
		}
	}

	void Release()
	{
		if (map)
		{
			delete[] map;
			map = nullptr;
			mapSize = { 0,0,0 };
		}
	}
};
struct Render
{
	std::vector<Drawable>	drawQueue;
	Drawable * 				screen = nullptr;
	Vec						screenSize;

	void alloc()
	{
		screen = new Drawable[screenSize.x * screenSize.y];
		for (int i = 0; i < screenSize.x; i++)
		{
			for (int k = 0; k < screenSize.y; k++)
			{
				screen[k * screenSize.x + i] = { ' ', i, k, -10, 7 };
			}
		}
	}
	void Release()
	{
		if (screen)
		{
			screenSize = { 0,0,0 };

		}
	}
	void Clear()
	{
		drawQueue.clear();
		for (int i = 0; i < screenSize.x; i++)
		{
			for (int k = 0; k < screenSize.y; k++)
			{
				screen[k * screenSize.x + i] = { ' ', i, k, -10 };
			}
		}
	}
	void Draw(const Drawable & d)
	{
		drawQueue.push_back(d);
	}
	void Flush(bool needsReDraw = true)
	{
		if (needsReDraw)
		{
			system("CLS");
			for (auto & d : drawQueue)
			{
				if (screen[d.pos.y * screenSize.x + d.pos.x].pos.z <= d.pos.z)
					screen[d.pos.y * screenSize.x + d.pos.x] = d;
			}
			for (int i = 0; i < screenSize.y; i++)
			{
				for (int k = 0; k < screenSize.x; k++)
				{
					rlutil::setColor((screen[i * screenSize.x + k].color));
					std::cout << screen[i * screenSize.x + k].spr;
				}
				std::cout << "\n";
			}
		}
		Clear();
	}
};

void LoadMap(const std::string & path, Map * map);

bool Move(Drawable & c, Map & map);

int main()
{
	rlutil::saveDefaultColor();
	Drawable c;
	c.spr = '@';
	c.pos = { 11, 5, 0 };
	c.color = RED;
	Render machine;
	Map map;
	LoadMap("Assets/Map.txt", &map);
	machine.screenSize.x = map.mapSize.x;
	machine.screenSize.y = map.mapSize.y;
	machine.alloc();
	map.Draw(&machine.drawQueue);
	machine.Draw(c);
	machine.Flush();



	while (!GetAsyncKeyState(VK_ESCAPE))
	{


		//Update
		bool redraw = Move(c, map);

		//Draw
		map.Draw(&machine.drawQueue);
		machine.Draw(c);
		machine.Flush(redraw);
	}




	map.Release();
	machine.Release();

	return 0;
}

void LoadMap(const std::string & path, Map * map)
{
	map->Release();
	
	std::ifstream in;
	in.open(path);
	in >> map->mapSize.x;
	in >> map->mapSize.y;
	in.ignore();

	map->alloc();

	std::string line;
	int counter = 0;
	while (std::getline(in, line))
	{
		for (int i = 0; i < map->mapSize.x; i++)
		{
			int color = 7;
			if (line[i] == '.')
				color = LIGHT_GREEN;
			else if (line[i] == '#')
				color = PURPLE;
			map->map[counter * map->mapSize.x + i].spr = line[i];
			map->map[counter * map->mapSize.x + i].color = color;
		}
		counter++;
	}

	in.close();
}

bool Move(Drawable & c, Map & map)
{
	static bool MovedLastFrame = false;
	static Timer moveTimer(true);

	bool moveThisFrame = true;
	bool redraw = false;

	Vec moveDir = {0,0,0};

	if (GetAsyncKeyState(VK_UP))
		moveDir.y = -1;
	else if (GetAsyncKeyState(VK_DOWN))
		moveDir.y = 1;
	else if (GetAsyncKeyState(VK_RIGHT))
		moveDir.x = 1;
	else if (GetAsyncKeyState(VK_LEFT))
		moveDir.x = -1;
	else
		moveThisFrame = false;

	if ((!MovedLastFrame || moveTimer.Peek() > 0.2) && moveThisFrame)
	{
		moveTimer.Stop();
		Vec newPos = c.pos + moveDir;
		newPos.z = 0;
		if (newPos.x >= 0 && newPos.x < map.mapSize.x && newPos.y >= 0 && newPos.y < map.mapSize.y)
		{
			c.pos = newPos;
			redraw = true;
		}
	}

	MovedLastFrame = moveThisFrame;
	return redraw;
}
