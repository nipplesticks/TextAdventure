#include "LevelEditor.h"

LevelEditor::LevelEditor()
{
	m_window = new sf::RenderWindow(sf::VideoMode(1900, 1060), "Level Editor");
	m_mapSize = {500, 500};
	m_pixelSize = { 30, 30 };
	
	m_camPos = { 64, 48};
	m_camSize = {64, 48};

	m_startPos = { 0, 0 };
	m_endPos = { 0, 0 };

	m_map = new Button[m_mapSize.x * m_mapSize.y];

	for (int y = 0; y < m_mapSize.y; y++)
	{
		for (int x = 0; x < m_mapSize.x; x++)
		{
			m_map[y * m_mapSize.x + x].setSize(m_pixelSize.x, m_pixelSize.y);
			m_map[y * m_mapSize.x + x].setPosition(m_pixelSize.x * x, m_pixelSize.y * y);
			m_map[y * m_mapSize.x + x].setColors(sf::Color(0, 0, 0, 255), sf::Color(128, 128, 128, 255), sf::Color(0, 0, 255, 255));
			m_map[y * m_mapSize.x + x].setText(",");
			m_map[y * m_mapSize.x + x].setTextColor(sf::Color(0, 128, 0, 255));
			m_map[y * m_mapSize.x + x].setTextSize(64.0f);
		}
	}
}

LevelEditor::~LevelEditor()
{
}

bool LevelEditor::WinodwIsOpen()
{
	return m_window->isOpen();
}

void LevelEditor::Update(float dt)
{
	sf::Event e;
	while (m_window->pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
			m_window->close();
	}
	static sf::Vector2i Mouse_Last_Frame = { 0,0 };
	sf::Vector2i Mouse_This_Frame = sf::Mouse::getPosition(*m_window);
	Mouse_This_Frame.x /= m_pixelSize.x;
	Mouse_This_Frame.y /= m_pixelSize.y;
	Mouse_This_Frame.x -= m_camPos.x;
	Mouse_This_Frame.y -= m_camPos.y;

	if (Mouse_Last_Frame.x >= 0 && Mouse_Last_Frame.y >= 0 &&
		Mouse_Last_Frame.x < m_mapSize.x && Mouse_Last_Frame.y < m_mapSize.y)
	{
		m_map[Mouse_Last_Frame.y * m_mapSize.x + Mouse_Last_Frame.x].setState(Button::Button_State::Standard);

	}
	
	if (Mouse_This_Frame.x >= 0 && Mouse_This_Frame.y >= 0 &&
		Mouse_This_Frame.x < m_mapSize.x && Mouse_This_Frame.y < m_mapSize.y)
	{

		m_map[Mouse_This_Frame.y * m_mapSize.x + Mouse_This_Frame.x].setState(Button::Button_State::Hover);
	}
	
	Mouse_Last_Frame = Mouse_This_Frame;
}

void LevelEditor::Draw()
{
	m_window->clear();
	for (int y = m_camPos.y; y < m_camPos.y + m_camSize.y; y++)
	{
		for (int x = m_camPos.x; x < m_camPos.x + m_camSize.x; x++)
		{

			if (x >= 0 && y >= 0 && x < m_mapSize.x && y < m_mapSize.y)
			{
				const sf::Vector2f & pos = m_map[y * m_mapSize.x + x].getPosition();
				m_map[y * m_mapSize.x + x].setPosition(pos.x - x * m_pixelSize.x, pos.y - y * m_pixelSize.y);
				m_window->draw(m_map[y * m_mapSize.x + x]);
			}
		}
	}
	m_window->display();
}
