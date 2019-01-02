#include "TextField.h"

sf::Font TextField::s_font;
bool TextField::s_fontLoaded = false;

TextField::TextField()
{
	if (!s_fontLoaded)
	{
		s_font.loadFromFile("Assets/font.ttf");
		s_fontLoaded = true;
	}
	m_background.setSize(sf::Vector2f(100, 50));
	m_background.setFillColor(sf::Color::White);
	m_background.setOutlineThickness(3.0f);
	m_background.setOutlineColor(sf::Color::Black);

	m_atBar.setSize(sf::Vector2f(1.0f, 45.0f));
	m_atBar.setPosition(3.0f, 2.5f);
	m_atBar.setFillColor(sf::Color::Black);

	m_text.setFont(s_font);
	m_text.setFillColor(sf::Color::Black);
	m_drawAtBar = false;
	m_selected = false;
}

void TextField::addChar(const char & c)
{
	m_currentText.insert(m_currentText.begin() + m_targetIndex++, c);
	m_timer = 2.0f;
	_updateTextAndPosition();
}

void TextField::RemoveChar(bool withDel)
{
	if (m_currentText.size() != 0)
	{
		m_timer = 2.0f;
		if (withDel && m_currentText.size() != m_targetIndex)
		{
			m_currentText.erase(m_currentText.begin() + m_targetIndex);
		}
		else if (m_targetIndex != 0)
		{
			if (m_targetIndex == m_currentText.size())
			{
				m_currentText.pop_back();
				m_targetIndex--;
			}
			else
			{
				m_currentText.erase(m_currentText.begin() + --m_targetIndex);
			}
		}
		_updateTextAndPosition();
	}
}

void TextField::moveTarget(int dir)
{
	m_targetIndex += dir;

	if (m_targetIndex < 0) m_targetIndex = 0;
	if (m_targetIndex > m_currentText.size()) m_targetIndex = m_currentText.size();
	m_timer = 2.0f;
	_updateTextAndPosition();
}

bool TextField::isPointInside(float x, float y) const
{
	return x > m_background.getPosition().x && x < m_background.getPosition().x + m_background.getSize().x &&
		y > m_background.getPosition().y && y < m_background.getPosition().y + m_background.getSize().y;
}

void TextField::setSelection(bool selectionState)
{
	m_selected = selectionState;
}

bool TextField::getSelectionState() const
{
	return m_selected;
}

void TextField::setPosition(float x, float y)
{
	m_background.setPosition(x, y);
	_updateTextAndPosition();
}

void TextField::setSize(float x, float y)
{
	m_background.setSize(sf::Vector2f(x, y));
	_updateTextAndPosition();
}

void TextField::setCharacterSize(float size)
{
	m_text.setCharacterSize(size);
	_updateTextAndPosition();
}

const sf::Vector2f & TextField::getSize() const
{
	return m_background.getSize();
}

const sf::Vector2f & TextField::getPosition() const
{
	return m_background.getPosition();
}

void TextField::Press(float x, float y)
{
	const sf::Vector2f & pos = m_background.getPosition();
	const sf::Vector2f & size = m_background.getSize();
	if (isPointInside(x, y))
	{
		m_timer = 0.0f;
		sf::FloatRect textPos = m_text.getGlobalBounds();
		if (x < textPos.left + 5.0f)
		{
			m_offset = 0.0f;
			m_targetIndex = 0;
		}
		else if (x > textPos.left + textPos.width - 5.0f)
		{
			m_offset = textPos.width;
			m_targetIndex = m_currentText.size();
		}
		else
		{
			float index = (x - textPos.left) / textPos.width;

			int realIndex = index * m_currentText.size();
			int realIndex2 = realIndex - 1;

			if (realIndex2 < 0)
				realIndex2 = realIndex;
			std::string sub1, sub2;
			sf::Text tmp1, tmp2;
			tmp1.setFont(s_font);
			tmp1.setCharacterSize(m_text.getCharacterSize());
			tmp2.setFont(s_font);
			tmp2.setCharacterSize(m_text.getCharacterSize());

			sub1 = m_currentText.substr(0, (int)realIndex);
			sub2 = m_currentText.substr(0, (int)realIndex2);

			tmp1.setString(sub1);
			tmp2.setString(sub2);

			float length1, length2, lengthReal;
			lengthReal = x - textPos.left;
			length1 = tmp1.getGlobalBounds().width;
			length2 = tmp2.getGlobalBounds().width;

			float dist1, dist2;
			dist1 = lengthReal - length1;
			dist2 = lengthReal - length2;

			m_offset = length2;
			m_targetIndex = realIndex2;
			if (dist1 < dist2)
			{
				m_offset = length1;
				m_targetIndex = realIndex;
			}

		}
		_updateTextAndPosition();
	}
}

void TextField::Update(float dt)
{
	m_timer += dt;

	if (m_selected && (int)m_timer % 2 == 0)
		m_drawAtBar = true;
	else
		m_drawAtBar = false;


	if (m_timer > 100.0f)
		m_timer -= 100.0f;
}

const std::string & TextField::getString() const
{
	return m_currentText;
}

void TextField::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_background, states);
	target.draw(m_text, states);
	if (m_drawAtBar)
		target.draw(m_atBar, states);
}

void TextField::_updateTextAndPosition()
{
	sf::FloatRect beforeBox = m_text.getLocalBounds();
	m_text.setString(m_currentText);
	sf::FloatRect tBox = m_text.getLocalBounds();

	const sf::Vector2f & pos = m_background.getPosition();
	const sf::Vector2f & size = m_background.getSize();
	sf::Vector2f origin = { 0, tBox.height - tBox.top * 0.5f };
	m_text.setOrigin(origin);
	m_text.setPosition(pos.x + 1.0f, pos.y + size.y * 0.5f);
	m_atBar.setSize(sf::Vector2f(3, size.y * 0.9f));
	m_atBar.setPosition(pos.x, pos.y * 1.1f);
	const sf::Vector2f & atBarPos = m_atBar.getPosition();
	m_offset += tBox.width - beforeBox.width;

	sf::Text tmp;
	tmp.setFont(s_font);
	tmp.setCharacterSize(m_text.getCharacterSize());
	std::string sub = m_currentText.substr(0, m_targetIndex);
	tmp.setString(sub);
	m_offset = tmp.getGlobalBounds().width;
	m_atBar.setPosition(atBarPos.x + m_offset, atBarPos.y);
}
