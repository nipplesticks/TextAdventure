#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Timer.h"
#include <stdlib.h>
#include "rlutil.h"
#pragma comment(lib, "user32")

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

const char WALL = '#';
const char DOOR_CLOSED = '=';
const char DOOR_OPEN = '/';
const char GRASS = ',';
const char FLOOR = '.';
const char WATER = '~';

void cls();
void setCursorPosition(int x, int y);

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
	bool operator==(const Vec & other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}
	bool operator!=(const Vec & other) const
	{
		return x != other.x || y != other.y || z != other.z;
	}

};
struct Drawable
{
	char spr = ' ';
	Vec pos = { 0,0,0 };
	int color = 7;
	bool NeedRedraw = false;
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

	void Draw(std::vector<Drawable*> * drawQueue)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			for (int k = 0; k < mapSize.y; k++)
			{
				drawQueue->push_back(&map[k * mapSize.x + i]);
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
	std::vector<Drawable*>	drawQueue;
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
		for (auto & t : drawQueue)
			t->NeedRedraw = false;
		drawQueue.clear();
	}
	void Draw(Drawable * d)
	{
		drawQueue.push_back(d);
	}
	void Flush()
	{
		static bool FIRST_DRAW = true;
		
		for (auto & d : drawQueue)
		{
			screen[d->pos.y * screenSize.x + d->pos.x] = *d;
		}
		if (FIRST_DRAW)
		{
			FIRST_DRAW = FALSE;
			setCursorPosition(0, 0);
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
		else
		{
			for (int i = 0; i < screenSize.y; i++)
			{
				for (int k = 0; k < screenSize.x; k++)
				{
					Drawable & target = screen[i * screenSize.x + k];
					if (target.NeedRedraw)
					{
						rlutil::setColor(target.color);
						setCursorPosition(target.pos.x, target.pos.y);
						std::cout << target.spr;
					}
				}
			}
		}
		setCursorPosition(screenSize.x, screenSize.y);
		Clear();
	}
};

void LoadMap(const std::string & path, Map * map);
Vec Move(Drawable & c, Map & map);
void Interact(Drawable & c, Map & map, const Vec & dir);

int main()
{
	SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
	rlutil::saveDefaultColor();
	Drawable c;
	c.spr = '@';
	c.color = RED;
	c.pos = { 11, 8, 0 };
	Render machine;
	Map map;
	LoadMap("Assets/Map.txt", &map);
	machine.screenSize.x = map.mapSize.x;
	machine.screenSize.y = map.mapSize.y;
	machine.alloc();
	
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		bool reDraw = false;
		//Update
		Vec dir = Move(c, map);
		Interact(c, map, dir);
		//Draw
		map.Draw(&machine.drawQueue);
		machine.Draw(&c);
		machine.Flush();
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

	int counter = 0;
	std::string line;
	while (std::getline(in, line))
	{
		for (int i = 0; i < map->mapSize.x; i++)
		{
			int color = 7;
			if (line[i] == GRASS)
				color = GREEN;
			else if (line[i] == WALL)
				color = PURPLE;
			else if (line[i] == FLOOR)
				color = WHITE;
			else if (line[i] == DOOR_CLOSED || line[i] == DOOR_OPEN)
				color = LIGHT_PURPLE;
			else if (line[i] == WATER)
				color = AQUA;

			map->map[counter * map->mapSize.x + i].spr = line[i];
			map->map[counter * map->mapSize.x + i].color = color;
		}
		counter++;
	}

	in.close();
}

Vec Move(Drawable & c, Map & map)
{
	static bool MovedLastFrame = false;
	static Timer moveTimer(true);

	bool moveThisFrame = false;

	Vec moveDir = {0,0,0};

	if (GetAsyncKeyState(VK_UP))
	{
		moveThisFrame = true;
		moveDir.y = -1;
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		moveDir.y = 1;
		moveThisFrame = true;
	}
	
	if (GetAsyncKeyState(VK_RIGHT))
	{
		moveThisFrame = true;
		moveDir.x = 1;
	}
	else if (GetAsyncKeyState(VK_LEFT))
	{
		moveDir.x = -1;
		moveThisFrame = true;
	}

	if ((!MovedLastFrame || moveTimer.Peek() > 0.1) && moveThisFrame)
	{
		moveTimer.Stop();
		Vec newPos = c.pos + moveDir;
		newPos.z = 0;
		if (newPos.x >= 0 && newPos.x < map.mapSize.x && newPos.y >= 0 && newPos.y < map.mapSize.y)
		{
			char type = map.map[newPos.y * map.mapSize.x + newPos.x].spr;
			if (type != WALL && type != DOOR_CLOSED && type != WATER)
			{
				map.map[c.pos.y * map.mapSize.x + c.pos.x].NeedRedraw = true;
				c.pos = newPos;
				c.NeedRedraw = true;
			}
		}
	}

	MovedLastFrame = moveThisFrame;
	return moveDir;
}

void Interact(Drawable & c, Map & map, const Vec & dir)
{
	static const Vec EMPTY = { 0,0,0 };
	static Vec lastDir = { 0,0,0 };
	static bool InteractedLastFrame = false;
	bool interactThisFrame = false;

	if (dir != EMPTY)
		lastDir = dir;

	if (GetAsyncKeyState('E'))
		interactThisFrame = true;

	if (!InteractedLastFrame && interactThisFrame && lastDir != EMPTY)
	{
		Vec InterActAt = c.pos + lastDir;
		if (InterActAt.x >= 0 && InterActAt.x < map.mapSize.x && InterActAt.y >= 0 && InterActAt.y < map.mapSize.y)
		{
			char type = map.map[InterActAt.y * map.mapSize.x + InterActAt.x].spr;
			if (type == DOOR_CLOSED)
			{
				map.map[InterActAt.y * map.mapSize.x + InterActAt.x].spr = DOOR_OPEN;
				map.map[InterActAt.y * map.mapSize.x + InterActAt.x].NeedRedraw = true;
			}
			else if (type == DOOR_OPEN)
			{
				map.map[InterActAt.y * map.mapSize.x + InterActAt.x].spr = DOOR_CLOSED;
				map.map[InterActAt.y * map.mapSize.x + InterActAt.x].NeedRedraw = true;
			}
		}
	}
	InteractedLastFrame = interactThisFrame;
}

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

void cls()
{
	// Get the Win32 handle representing standard output.
// This generally only has to be done once, so we make it static.
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };

	// std::cout uses a buffer to batch writes to the underlying console.
	// We need to flush that to the console because we're circumventing
	// std::cout entirely; after we clear the console, we don't want
	// stale buffered text to randomly be written out.
	std::cout.flush();

	// Figure out the current width and height of the console window
	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		// TODO: Handle failure!
		abort();
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;

	// Flood-fill the console with spaces to clear it
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

	// Reset the attributes of every character to the default.
	// This clears all background colour formatting, if any.
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

	// Move the cursor back to the top left for the next sequence of writes
	SetConsoleCursorPosition(hOut, topLeft);
}

void setCursorPosition(int x, int y)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}
