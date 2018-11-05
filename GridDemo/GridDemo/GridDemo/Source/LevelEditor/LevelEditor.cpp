#include "LevelEditor.h"

LevelEditor::LevelEditor()
{
	sf::Vector2i windowSize = { 1200, 720 };
	m_window = new sf::RenderWindow(sf::VideoMode(windowSize.x, windowSize.y), "Level Editor");
	m_mapSize = {200, 100};
	m_pixelSize = { 30, 30 };
	
	m_camPos = {0, 0};
	m_camSize = {(windowSize.x / m_pixelSize.x) * 0.8f, (windowSize.y / m_pixelSize.y) * 0.8f};


	m_RightPanel.setPosition(m_camSize.x * m_pixelSize.x, 0);
	m_RightPanel.setSize(sf::Vector2f(windowSize) - m_RightPanel.getPosition());
	m_RightPanel.setFillColor(sf::Color(128, 128, 128, 255));
	m_RightPanel.setOutlineThickness(-5.0f);
	m_RightPanel.setOutlineColor(sf::Color(32, 32, 32, 255));

	const sf::Vector2f & pos = m_RightPanel.getPosition();
	const sf::Vector2f & size = m_RightPanel.getSize();
	m_fieldOne.setPosition(pos.x + 5.0f, pos.y + 5.0f);
	m_fieldOne.setSize(size.x - 10.0f, 25.0f);
	m_fieldOne.setCharacterSize(16.0f);
	
	m_fieldOne.addChar('T');
	m_fieldOne.addChar('e');
	m_fieldOne.addChar('s');
	m_fieldOne.addChar('t');

	m_BottomPanel.setPosition(0, m_camSize.y * m_pixelSize.y);

	m_BottomPanel.setSize(sf::Vector2f(
		m_RightPanel.getPosition().x - m_RightPanel.getOutlineThickness(),
		(sf::Vector2f(windowSize) - m_BottomPanel.getPosition()).y
	)
	);

	m_BottomPanel.setFillColor(sf::Color(128, 128, 128, 255));
	m_BottomPanel.setOutlineThickness(-5.0f);
	m_BottomPanel.setOutlineColor(sf::Color(32, 32, 32, 255));


	m_startPos = { 0, 0 };
	m_endPos = { 0, 0 };

	m_map = new Button[m_mapSize.x * m_mapSize.y];

	for (int y = 0; y < m_mapSize.y; y++)
	{
		for (int x = 0; x < m_mapSize.x; x++)
		{
			m_map[y * m_mapSize.x + x].setSize(m_pixelSize.x, m_pixelSize.y);
			m_map[y * m_mapSize.x + x].setWorldPosition(m_pixelSize.x * x, m_pixelSize.y * y);
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

	m_fieldOne.Update(dt);


	_input();
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
	m_window->draw(m_fieldOne);
	m_window->display();
}

void LevelEditor::_input()
{
	m_MouseThisFrame = sf::Mouse::getPosition(*m_window);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		_mapMove();
	}
	else
	{
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
		//m_camPos.y -= m_camSize.y;
		m_fieldOne.addChar('M');
	}
	else if (Down_now && !Down_last)
	{
		//m_camPos.y += m_camSize.y;
		m_fieldOne.RemoveChar();
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
	static sf::Vector2f indexLastFrame = { 0,0 };


	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		m_fieldOne.Press(m_MouseThisFrame.x, m_MouseThisFrame.y);
	}


	sf::Vector2f indexThisFame = sf::Vector2f(m_MouseThisFrame);
	indexThisFame.x /= m_pixelSize.x;
	indexThisFame.y /= m_pixelSize.y;
	indexThisFame.x += m_camPos.x;
	indexThisFame.y += m_camPos.y;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		int i = 0;

	if (indexLastFrame.x >= 0 && indexLastFrame.y >= 0 &&
		indexLastFrame.x < m_mapSize.x && indexLastFrame.y < m_mapSize.y)
	{
		m_map[(int)indexLastFrame.y * m_mapSize.x + (int)indexLastFrame.x].setState(Button::Button_State::Standard);

	}

	if (indexThisFame.x >= 0 && indexThisFame.y >= 0 &&
		indexThisFame.x < m_mapSize.x && indexThisFame.y < m_mapSize.y)
	{

		m_map[(int)indexThisFame.y * m_mapSize.x + (int)indexThisFame.x].setState(Button::Button_State::Hover);
	}

	indexLastFrame = indexThisFame;
}
