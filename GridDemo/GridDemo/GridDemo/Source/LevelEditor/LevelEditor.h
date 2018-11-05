#pragma once
#include "Source/GUI/Button.h"
class LevelEditor
{
private:
	sf::RenderWindow * m_window;
	Button * m_map;
	sf::Vector2i m_mapSize;
	sf::Vector2i m_pixelSize;

	// Tools
	sf::Vector2i m_startPos;
	sf::Vector2i m_endPos;

	// Camera
	sf::Vector2i m_camPos;
	sf::Vector2i m_camSize;
public:
	LevelEditor();
	~LevelEditor();


	bool WinodwIsOpen();
	void Update(float dt);
	void Draw();
private:
	void _init();
};