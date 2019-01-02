#include "LevelEditor.h"

LevelEditor::LevelEditor()
{
#ifdef _DEBUG
	sf::Vector2i windowSize = { 1200, 720 };
	m_window = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y), "Level Editor");
#else
	sf::Vector2i windowSize = { 1920, 1080 };
	m_window = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y), "Level Editor", sf::Style::Fullscreen);
#endif //  DEBUG

	
	
	m_mapSize = {201, 101};
	m_pixelSize = { 30, 30 };
	
	m_camPos = {0, 0};
	m_camSize = {(windowSize.x / m_pixelSize.x) * 0.8f, (windowSize.y / m_pixelSize.y) * 0.8f};

	_initPanelsAndTextFields();
	_initMap();
	_initTools();
}

LevelEditor::~LevelEditor()
{
	delete m_window;
	m_window = nullptr;
	delete[] m_map;
	m_map = nullptr;
}

bool LevelEditor::WinodwIsOpen()
{
	return m_window->isOpen();
}

void LevelEditor::Update(float dt)
{
	static bool s_dirButtonPressedLastFrame = false;
	bool dirButtonPressedThisFrame;

	sf::Event e;
	while (m_window->pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
			m_window->close();
		else
		{
			if (m_textFieldSelection != -1)
			{
				if (e.type == sf::Event::TextEntered)
				{
					
					if (TextField::isValidKey(e.text.unicode))
						m_textFields[m_textFieldSelection].addChar(static_cast<char>(e.text.unicode));
					else if (e.text.unicode == 8) // Back
						m_textFields[m_textFieldSelection].RemoveChar();
				}
				else if (e.type != sf::Event::KeyReleased)
				{
					bool shiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);


					if (dirButtonPressedThisFrame = e.key.code == sf::Keyboard::Right)
					{
						if (shiftPressed && dirButtonPressedThisFrame && !s_dirButtonPressedLastFrame)
						{
							m_textFields[m_textFieldSelection].BeginSelection();
						}
						m_textFields[m_textFieldSelection].moveTarget(1, shiftPressed);
					}
					else if (dirButtonPressedThisFrame = e.key.code == sf::Keyboard::Left)
					{
						if (shiftPressed && dirButtonPressedThisFrame && !s_dirButtonPressedLastFrame)
						{
							m_textFields[m_textFieldSelection].BeginSelection();
						}
						m_textFields[m_textFieldSelection].moveTarget(-1, shiftPressed);
					}
					else if (e.key.code == sf::Keyboard::Delete)
					{
						m_textFields[m_textFieldSelection].RemoveChar(true);
					}

					

					s_dirButtonPressedLastFrame = dirButtonPressedThisFrame;
				}	
			}
		}
	}

	_input();

	for (int i = 0; i < NUMBER_OF_TEXT_FIELDS; i++) 
		m_textFields[i].Update(dt);
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
				const sf::Vector2f & pos = m_map[y * m_mapSize.x + x].getWorldPosition();
				sf::Vector2f viewPos = { pos.x - m_camPos.x * m_pixelSize.x, pos.y - m_camPos.y * m_pixelSize.y };
				m_map[y * m_mapSize.x + x].setViewPosition(viewPos.x, viewPos.y);
				m_window->draw(m_map[y * m_mapSize.x + x]);
			}
		}
	}

	m_window->draw(m_BottomPanel);
	m_window->draw(m_RightPanel);

	for (int i = 0; i < NUMBER_OF_TEXT_FIELDS; i++)
		m_window->draw(m_textFields[i]);

	if (m_selectionInfo.getDataString() == "DRAW")
		m_window->draw(m_selectionInfo);

	m_window->draw(m_selectionPos);

	m_window->display();
}

void LevelEditor::_initPanelsAndTextFields()
{
	m_RightPanel.setPosition(m_camSize.x * m_pixelSize.x, 0);
	m_RightPanel.setSize(sf::Vector2f(m_window->getSize()) - m_RightPanel.getPosition());
	m_RightPanel.setFillColor(sf::Color(128, 128, 128, 255));
	m_RightPanel.setOutlineThickness(-5.0f);
	m_RightPanel.setOutlineColor(sf::Color(32, 32, 32, 255));

	const sf::Vector2f & pos = m_RightPanel.getPosition();
	const sf::Vector2f & size = m_RightPanel.getSize();

	m_textFields[0].setPosition(pos.x + 5.0f, pos.y + 5.0f);
	m_textFields[0].setSize(size.x - 10.0f, 25.0f);
	m_textFields[0].setCharacterSize(16.0f);

	m_textFields[0].addChar('T');
	m_textFields[0].addChar('e');
	m_textFields[0].addChar('s');
	m_textFields[0].addChar('t');

	m_textFieldSelection = -1;

	m_BottomPanel.setPosition(0, m_camSize.y * m_pixelSize.y);

	m_BottomPanel.setSize(sf::Vector2f(
		m_RightPanel.getPosition().x - m_RightPanel.getOutlineThickness(),
		(sf::Vector2f(m_window->getSize()) - m_BottomPanel.getPosition()).y
	)
	);

	m_BottomPanel.setFillColor(sf::Color(128, 128, 128, 255));
	m_BottomPanel.setOutlineThickness(-5.0f);
	m_BottomPanel.setOutlineColor(sf::Color(32, 32, 32, 255));

}

void LevelEditor::_initMap()
{
	m_map = new Button[m_mapSize.x * m_mapSize.y];

	for (int y = 0; y < m_mapSize.y; y++)
	{
		for (int x = 0; x < m_mapSize.x; x++)
		{
			int index = y * m_mapSize.x + x;

			m_map[index].setSize(m_pixelSize.x, m_pixelSize.y);
			m_map[index].setWorldPosition(m_pixelSize.x * x, m_pixelSize.y * y);
			m_map[index].setColors(sf::Color(0, 0, 0, 255), sf::Color(128, 128, 128, 255), sf::Color(0, 0, 255, 255));
			m_map[index].setText(",");
			m_map[index].setTextColor(sf::Color(0, 128, 0, 255));
			m_map[index].setTextSize(64.0f);
			m_map[index].setDataString("WORLD#GRASS#0");
		}
	}
}

void LevelEditor::_initTools()
{
	m_startPos = { 0, 0 };
	m_endPos = { 0, 0 };

	m_selectionPos.setColors(sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent);
	m_selectionPos.setWorldPosition(30, 0);
	m_selectionPos.setViewPosition(30, 0);
	m_selectionPos.setTextColor(sf::Color(255, 255, 255, 200));
	m_selectionPos.setSize(30, 30);
	m_selectionPos.setTextSize(20);
	
	m_selectionInfo.setTextColor(sf::Color(255, 255, 255, 200));
	m_selectionInfo.setColors(sf::Color(40, 40, 40, 230), sf::Color(40, 40, 40, 230), sf::Color(40, 40, 40, 230));
	m_selectionInfo.setOutlineColor(sf::Color(255, 255, 255, 200));
	m_selectionInfo.setOutlineThickness(2.0f);
	m_selectionInfo.setTextSize(16.0f);
}

void LevelEditor::_initFillTools()
{
}

void LevelEditor::_initTileTools()
{
}

void LevelEditor::_input()
{
	m_MouseThisFrame = sf::Mouse::getPosition(*m_window);

	// Reset input values
	if (m_indexLastFrame.x >= 0 && m_indexLastFrame.y >= 0 &&
		m_indexLastFrame.x < m_mapSize.x && m_indexLastFrame.y < m_mapSize.y)
	{
		m_map[(int)m_indexLastFrame.y * m_mapSize.x + (int)m_indexLastFrame.x].setState(Button::Button_State::Standard);
	}
	m_selectionInfo.setDataString("!DRAW");
	// Reset end

	if (m_textFieldSelection == -1 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		_mapMove();
	}
	else
	{
		if (m_textFieldSelection == -1)
			_handleKeyboardInput();
		
		
		_handleMouseInput();
	}

	m_MouseLastFrame = m_MouseThisFrame;
}

void LevelEditor::_mapMove()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{	
		sf::Vector2f direction = sf::Vector2f(m_MouseThisFrame - m_MouseLastFrame);
		direction.x /= (float)m_pixelSize.x;
		direction.y /= (float)m_pixelSize.y;
		m_camPos -= direction;
	}
}

void LevelEditor::_handleKeyboardInput()
{
	static bool Up_last		= false;
	static bool Left_last	= false;
	static bool Right_last	= false;
	static bool Down_last	= false;

	bool Up_now		= false;
	bool Left_now	= false;
	bool Right_now	= false;
	bool Down_now	= false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		m_window->close();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		Up_now = true;
		
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		Down_now = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		Left_now = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		Right_now = true;
	}

	
	if (Up_now && !Up_last)
	{
		m_camPos.y -= m_camSize.y;
	}
	else if (Down_now && !Down_last)
	{
		m_camPos.y += m_camSize.y;
	}
	if (Left_now && !Left_last)
	{
		m_camPos.x -= m_camSize.x;
	}
	else if (Right_now && !Right_last)
	{
		m_camPos.x += m_camSize.x;
	}


	Up_last		= Up_now;
	Left_last	= Left_now;
	Right_last	= Right_now;
	Down_last	= Down_now;
}

void LevelEditor::_handleMouseInput()
{
	static bool s_PressedLastFrame = false;
	bool pressedThisFrame;

	if (pressedThisFrame = sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (!s_PressedLastFrame)
		{
			m_textFieldSelection = -1;
			for (int i = 0; i < NUMBER_OF_TEXT_FIELDS && m_textFieldSelection == -1; i++)
			{
				m_textFields[i].setSelection(false);
				if (m_textFields[i].isPointInside(m_MouseThisFrame.x, m_MouseThisFrame.y))
				{
					m_textFieldSelection = i;
					m_textFields[i].setSelection(true);
				}
			}
			for (int i = m_textFieldSelection + 1; i < NUMBER_OF_TEXT_FIELDS; i++)
			{
				m_textFields[i].setSelection(false);
			}
		}
		m_textFields[m_textFieldSelection].Press(m_MouseThisFrame.x, m_MouseThisFrame.y);
		if (!s_PressedLastFrame)
			m_textFields[m_textFieldSelection].BeginSelection();
	}

	s_PressedLastFrame = pressedThisFrame;

	sf::Vector2f indexThisFame = sf::Vector2f(m_MouseThisFrame);
	indexThisFame.x /= m_pixelSize.x;
	indexThisFame.y /= m_pixelSize.y;
	indexThisFame.x += m_camPos.x;
	indexThisFame.y += m_camPos.y;

	if (m_textFieldSelection == -1)
	{
		if (!_intersects(m_MouseThisFrame.x, m_MouseThisFrame.y, m_BottomPanel) && !_intersects(m_MouseThisFrame.x, m_MouseThisFrame.y, m_RightPanel))
		{
			if (indexThisFame.x >= 0 && indexThisFame.y >= 0 &&
				indexThisFame.x < m_mapSize.x && indexThisFame.y < m_mapSize.y)
			{
				std::string pos = std::to_string((int)indexThisFame.x) + ", " + std::to_string((int)indexThisFame.y);
				m_selectionPos.setText(pos);
				m_map[(int)indexThisFame.y * m_mapSize.x + (int)indexThisFame.x].setState(Button::Button_State::Hover);
				std::string data = m_map[(int)indexThisFame.y * m_mapSize.x + (int)indexThisFame.x].getDataString();
				data = _infoFromData(data);
				m_selectionInfo.setText(data, true);
				sf::Vector2f size = m_selectionInfo.getSize();
				m_selectionInfo.setViewPosition(m_MouseThisFrame.x - size.x, m_MouseThisFrame.y - size.y);
				m_selectionInfo.setDataString("DRAW");
			}
			m_indexLastFrame = indexThisFame;
		}
	}
}

std::string LevelEditor::_infoFromData(const std::string & data) const
{
	std::string type, name, id, output = "";

	int typeEnd = data.find_first_of('#');
	int nameEnd = data.find_last_of('#');
	type = data.substr(0, typeEnd);
	name = data.substr(typeEnd + 1, nameEnd - typeEnd - 1);
	id = data.substr(nameEnd + 1, data.size() - nameEnd - 1);

	if (type == "WORLD")
	{
		output = "Type: " + type + "\n";
		output += "Name: " + name + "\n";

		if (name == "GRASS")
			output += "- Walkable";
		else if (name == "WATER" || name == "WALL")
			output += "- Not Walkable";
		else if (name == "DOOR")
			output += "- Interactable";
	}

	return output;
}

bool LevelEditor::_intersects(float x, float y, const sf::RectangleShape & rs) const
{
	return rs.getGlobalBounds().contains(sf::Vector2f(x, y));
}
