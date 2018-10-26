#include "Render.h"
#include "Extern.h"
#include <iostream>
#include "../Help_Headers/rlutil.h"
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

std::vector<Drawable*> EXT::g_drawQueue;

Render::Render()
{
	m_viewPort = { 0,0,0,0 };
	m_size = { 0,0,0 };
	m_renderTarget = nullptr;
}

void Render::Init(Quad viewport)
{
	m_viewPort = viewport;
	m_size.x = m_viewPort.width - m_viewPort.left;
	m_size.y = m_viewPort.height - m_viewPort.top;
	_alloc();
}

void Render::_alloc()
{
	m_renderTarget = new Drawable[m_size.x * m_size.y];
}

void Render::_setTarget(const int & x, const int & y)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

void Render::_cls()
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };
	std::cout.flush();
	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		abort();
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

	SetConsoleCursorPosition(hOut, topLeft);
}
