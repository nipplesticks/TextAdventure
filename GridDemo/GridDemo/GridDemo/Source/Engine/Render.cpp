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

	m_viewPort = viewport;
	m_size.x = m_viewPort.width;
	m_size.y = m_viewPort.height;
	_alloc();
}

void Render::Resize(Quad viewPort)
{
	_cleanup();
	Init(viewPort);
}

void Render::Flush(const Camera & cam, bool forceRedraw)
{
	if (forceRedraw)
	{
		_cls();
		
	}

	for (auto & d : EXT::g_drawQueue)
	{
		Vec viewPos = d->getPosition() - cam.getPosition();
		if (viewPos.x >= 0 && viewPos.x < m_size.x &&
			viewPos.y >= 0 && viewPos.y < m_size.y)
		{
			m_renderTarget[viewPos.y * m_size.x + viewPos.x] = *d;
			m_renderTarget[viewPos.y * m_size.x + viewPos.x].setPosition(viewPos);
		}
	}

	for (int y = 0; y < m_size.y; y++)
	{
		for (int x = 0; x < m_size.x; x++)
		{
			Drawable & target = m_renderTarget[y * m_size.x + x];
			if (target.isInNeedOfRedraw() || forceRedraw)
			{
				Vec screenPos = target.getPosition() + Vec{ m_viewPort.left, m_viewPort.top, 0 };
				rlutil::setColor(target.getColor());
				_setTarget(screenPos.x, screenPos.y);
				std::cout << target.getSprite();
				_setTarget(m_viewPort.left + m_viewPort.width, m_viewPort.top + m_viewPort.height);
			}
		}
	}
}

void Render::Clear()
{
	for (auto & t : EXT::g_drawQueue)
		t->setRedrawState(false);
	EXT::g_drawQueue.clear();
}

void Render::_cleanup()
{
	delete[] m_renderTarget;
	m_renderTarget = nullptr;
	m_viewPort = { 0,0,0,0 };
	m_size = { 0, 0, 0 };
}

void Render::_alloc()
{
	m_renderTarget = new Drawable[m_size.x * m_size.y];
	for (int i = 0; i < m_size.x; i++)
		for (int k = 0; k < m_size.y; k++)
			m_renderTarget[k * m_size.x + i] = Drawable(Sprite::NOTILE, Vec{ i, k, -10 }, Color::BLACK);
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
	/*static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

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

	SetConsoleCursorPosition(hOut, topLeft);*/

	for (int i = 0; i < m_size.x; i++)
		for (int k = 0; k < m_size.y; k++)
		{
			m_renderTarget[k * m_size.x + i] = Drawable(Sprite::NOTILE, Vec{ i, k, -10 }, Color::BLACK);
			m_renderTarget[k * m_size.x + i].setRedrawState(false);
		}
}
