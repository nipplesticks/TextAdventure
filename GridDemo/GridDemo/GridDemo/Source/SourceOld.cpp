#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Help_Headers/Timer.h"
#include <stdlib.h>
#include "Help_Headers/rlutil.h"
#include "Help_Headers/Colors.h"
#include "Help_Headers/Sprites.h"
#include "Help_Headers/Structs.h"
#pragma comment(lib, "user32")

void cls();
void setCursorPosition(int x, int y);

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
	void Flush(Vec * camera, bool redrawAll = false)
	{
		static bool FIRST_DRAW = true;
		
		if (redrawAll)
		{
			for (int i = 0; i < screenSize.y; i++)
			{
				for (int k = 0; k < screenSize.x; k++)
				{
					screen[i * screenSize.x + k].spr = Sprite::NOTILE;
					screen[i * screenSize.x + k].pos.z = -10;
				}
			}
		}

		for (auto & d : drawQueue)
		{
			Vec viewPos = d->pos - *camera;
			if (viewPos.x >= 0 && viewPos.x < screenSize.x && 
				viewPos.y >= 0 && viewPos.y < screenSize.y)
				screen[viewPos.y * screenSize.x + viewPos.x] = *d;
		}
		if (FIRST_DRAW || redrawAll)
		{
			FIRST_DRAW = false;
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
						Vec ViewPos = target.pos - *camera;
						rlutil::setColor(target.color);
						setCursorPosition(ViewPos.x, ViewPos.y);
						std::cout << target.spr;
					}
				}
			}
		}
		setCursorPosition(screenSize.x, screenSize.y);
		Clear();
	}
};

Drawable LoadMap(const std::string & path, Map * map);
Vec Move(Drawable & c, Map & map);
void Interact(Drawable & c, Map & map, const Vec & dir);
void InitWindow()
{
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;                   // Width of each character in the font
	cfi.dwFontSize.Y = 24;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	//std::wcscpy(cfi.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
	rlutil::saveDefaultColor();
}

int main()
{
	InitWindow();

	std::vector<Drawable> items;

	Drawable c;
	Vec camera = { 0, 0, 0 };

	Render machine;
	Map map;
	c = LoadMap("Assets/Map.txt", &map);
	machine.screenSize.x = 145;
	machine.screenSize.y = 44;
	machine.alloc();
	
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		bool reDraw = false;
		//Update
		Vec dir = Move(c, map);

		// Move camera
		bool redrawAll = false;
		if (c.pos.x >= machine.screenSize.x + camera.x)
		{
			redrawAll = true;
			camera.x += machine.screenSize.x;
		}
		else if (c.pos.x < camera.x)
		{
			redrawAll = true;
			camera.x -= machine.screenSize.x;
		}
		if (c.pos.y >= machine.screenSize.y + camera.y)
		{
			redrawAll = true;
			camera.y += machine.screenSize.y;
		}
		else if (c.pos.y < camera.y)
		{
			redrawAll = true;
			camera.y -= machine.screenSize.y;
		}


		Interact(c, map, dir);
		//Draw
		map.Draw(&machine.drawQueue);
		machine.Draw(&c);
		machine.Flush(&camera, redrawAll);
	}

	map.Release();
	machine.Release();

	return 0;
}

Drawable LoadMap(const std::string & path, Map * map)
{
	Drawable player;
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
			bool isMap = true;
			int color = 7;
			if (line[i] == Sprite::GRASS)
				color = Color::GREEN;
			else if (line[i] == Sprite::WALL)
				color = Color::PURPLE;
			else if (line[i] == Sprite::FLOOR)
				color = Color::WHITE;
			else if (line[i] == Sprite::DOOR_CLOSED || line[i] == Sprite::DOOR_OPEN)
				color = Color::LIGHT_PURPLE;
			else if (line[i] == Sprite::WATER)
				color = Color::AQUA;
			else if (line[i] == Sprite::ITEM)
			{
				color = Color::YELLOW;
				isMap = false;
			}
			else if (line[i] == Sprite::PLAYER)
			{
				color = Color::RED;
				isMap = false;
				player.pos.x = i;
				player.pos.y = counter;
				player.pos.z = 0;
				player.spr = Sprite::PLAYER;
				player.color = color;
			}

			if (isMap)
			{
				map->map[counter * map->mapSize.x + i].spr = line[i];
				map->map[counter * map->mapSize.x + i].color = color;
			}
			else
			{
				if (counter > 0 && counter < map->mapSize.y - 1 && i > 0 && i < map->mapSize.x - 1)
				{
					char around[4];
					around[0]	= map->map[(counter + 1) * map->mapSize.x + i].spr;
					around[1]	= map->map[(counter - 1) * map->mapSize.x + i].spr;
					around[2]	= map->map[counter * map->mapSize.x + i + 1].spr;
					around[3]	= map->map[counter * map->mapSize.x + i - 1].spr;

					int floor = 0;
					int grass = 0;
					for (int a = 0; a < 4; a++)
					{
						if (around[a] == Sprite::FLOOR)
							floor++;
						else if (around[a] == Sprite::GRASS)
							grass++;

						if (grass > floor)
						{
							map->map[counter * map->mapSize.x + i].spr = ',';
							map->map[counter * map->mapSize.x + i].color = Color::GREEN;
						}
						else
						{
							map->map[counter * map->mapSize.x + i].spr = '.';
							map->map[counter * map->mapSize.x + i].color = Color::WHITE;
						}
					}

				}
			}
		}
		counter++;
	}

	in.close();
	return player;
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
			if (type != Sprite::WALL && type != Sprite::DOOR_CLOSED && type != Sprite::WATER)
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
			if (type == Sprite::DOOR_CLOSED)
			{
				map.map[InterActAt.y * map.mapSize.x + InterActAt.x].spr = Sprite::DOOR_OPEN;
				map.map[InterActAt.y * map.mapSize.x + InterActAt.x].NeedRedraw = true;
			}
			else if (type == Sprite::DOOR_OPEN)
			{
				map.map[InterActAt.y * map.mapSize.x + InterActAt.x].spr = Sprite::DOOR_CLOSED;
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
