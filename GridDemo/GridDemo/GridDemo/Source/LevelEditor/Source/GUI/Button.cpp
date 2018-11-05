#include "Button.h"

sf::Font Button::s_font;
bool Button::s_fontLoaded = false;

Button::Button()
{
	if (!s_fontLoaded)
	{
		s_font.loadFromFile("Assets/font.ttf");
		s_fontLoaded = true;
	}

	m_text.setFillColor(sf::Color::Black);
	m_text.setFont(s_font);

	m_button.setPosition(0, 0);
	m_button.setSize(sf::Vector2f{ 100, 50 });
	m_button.setOutlineColor(sf::Color::Black);
	m_button.setOutlineThickness(-1.0f);

	m_state = Standard;

	m_colors[Standard] = sf::Color{ 255,0,0,255 };
	m_colors[Hover] = sf::Color{ 0,255,0,255 };
	m_colors[Press] = sf::Color{ 0,0,255,255 };

	m_button.setFillColor(m_colors[Standard]);
}

Button::~Button()
{

}

void Button::setText(const sf::String & text)
{
	m_text.setString(text);
	_allign();
}

void Button::setPosition(float x, float y)
{
	m_button.setPosition(x, y);
	_allign();
}

void Button::setSize(float x, float y)
{
	m_button.setSize(sf::Vector2f{ x, y });
	_allign();
}

void Button::setState(const Button_State & state)
{
	if (m_state != state)
	{
		m_state = state;
		m_button.setFillColor(m_colors[m_state]);
	}
}

const sf::Vector2f & Button::getPosition() const
{
	return m_button.getPosition();
}

const sf::Vector2f & Button::getSize() const
{
	return m_button.getSize();
}

const Button::Button_State & Button::getState() const
{
	return m_state;
}

void Button::setColors(const sf::Color & standard, const sf::Color & hover, const sf::Color & press)
{
	m_colors[Standard]	= standard;
	m_colors[Hover]		= hover;
	m_colors[Press]		= press;
}

bool Button::PointIsInside(float x, float y)
{
	const sf::Vector2f & pos = getPosition();
	const sf::Vector2f & size = getSize();


	return	x > pos.x &&
			x < pos.x + size.x &&
			y > pos.y &&
			y < pos.y + size.y;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_button, states);
	target.draw(m_text, states);
}

void Button::_allign()
{
	const sf::Vector2f & Qpos = getPosition();
	const sf::Vector2f & Qsize = getSize();

	//const sf::FloatRect & Tquad = m_text.getGlobalBounds();
	const sf::FloatRect & Tquad = m_text.getLocalBounds();

	sf::Vector2f origin = { (Tquad.left + Tquad.width) * 0.5f, (Tquad.top + Tquad.height) * 0.5f };
	m_text.setOrigin(origin);
	m_text.setPosition(Qpos + Qsize * 0.5f);
}