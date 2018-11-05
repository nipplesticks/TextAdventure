#pragma once
#include "Source/GUI/Button.h"
#include "Source/GUI/TextField.h"
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
	sf::Vector2f m_camPos;
	sf::Vector2f m_camSize;

	// Mouse
	sf::Vector2i m_MouseLastFrame;
	sf::Vector2i m_MouseThisFrame;

	// Panels
	sf::RectangleShape m_RightPanel;
	sf::RectangleShape m_BottomPanel;

	// Text
	TextField m_fieldOne;


public:
	LevelEditor();
	~LevelEditor();


	bool WinodwIsOpen();
	void Update(float dt);
	void Draw();
private:
	void _init();
	void _input();
	void _mapMove();
	void _handleKeyboardInput();
	void _handleMouseInput();
};