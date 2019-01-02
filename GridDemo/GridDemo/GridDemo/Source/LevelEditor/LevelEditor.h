#pragma once
#include "Source/GUI/Button.h"
#include "Source/GUI/TextField.h"
class LevelEditor
{
private:
	enum TileTool
	{
		None = 0,
		World,
		Enemy,
		Item
	};
	enum FillTool
	{
		Pencil,
		Line,
		Square,
		Bucket
	};
private:
	sf::RenderWindow * m_window;
	Button * m_map;
	sf::Vector2i m_mapSize;
	sf::Vector2i m_pixelSize;
	sf::Vector2f m_indexLastFrame = { 0.0f, 0.0f };

	// Tools
	Button m_selectionPos;
	Button m_selectionInfo;
	sf::Vector2i m_startPos;
	sf::Vector2i m_endPos;
	std::vector<Button> m_tileTools;
	std::vector<Button> m_fillTools;


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
	static const int NUMBER_OF_TEXT_FIELDS = 1;
	TextField m_textFields[NUMBER_OF_TEXT_FIELDS];
	int m_textFieldSelection;

public:
	LevelEditor();
	~LevelEditor();


	bool WinodwIsOpen();
	void Update(float dt);
	void Draw();
private:
	void _initPanelsAndTextFields();
	void _initMap();
	void _initTools();
	void _initFillTools();
	void _initTileTools();

	void _input();
	void _mapMove();
	void _handleKeyboardInput();
	void _handleMouseInput();
	std::string _infoFromData(const std::string & data) const;
	bool _intersects(float x, float y, const sf::RectangleShape & rs) const;
};