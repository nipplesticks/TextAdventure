#pragma once
#include <SFML/Graphics.hpp>

class Button : public sf::Drawable
{
public:
	enum Button_State
	{
		Standard = 0,
		Hover,
		Press
	};
private:
	static sf::Font s_font;
	static bool s_fontLoaded;

	sf::RectangleShape m_button;
	sf::Vector2f m_worldPos;
	Button_State m_state;
	sf::Color m_colors[3];
	sf::Text m_text;

	std::string m_data = "";

public:
	Button();
	~Button();

	void setText(const sf::String & text, bool fitButtonSizeToText = false);
	void setViewPosition(float x, float y);
	void setWorldPosition(float x, float y);
	void setSize(float x, float y);
	void setState(const Button_State & state);
	void setOutlineThickness(float size);


	void setOutlineColor(const sf::Color & oCol);

	const sf::Vector2f & getViewPosition() const;
	const sf::Vector2f & getWorldPosition() const;
	const sf::Vector2f & getSize() const;
	const Button_State & getState() const;

	void setDataString(const std::string & data);
	const std::string & getDataString() const;

	void setTextSize(float size);
	void setTextColor(const sf::Color & tCol);

	void setColors(const sf::Color & standard, const sf::Color & hover,	const sf::Color & press);
	bool PointIsInside(float x, float y);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	void _allign();
};