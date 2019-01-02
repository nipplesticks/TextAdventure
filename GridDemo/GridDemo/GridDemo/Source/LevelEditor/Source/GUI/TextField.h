#pragma once
#include <SFML/Graphics.hpp>
#include <string>
class TextField : public sf::Drawable
{
private:
	static sf::Font s_font;
	static bool s_fontLoaded;

	float m_offset = 0.0f;

	sf::RectangleShape m_atBar;
	bool m_drawAtBar;
	bool m_selected;

	sf::RectangleShape m_background;
	std::string m_currentText;
	sf::Text m_text;

	float m_timer = 0.0f;
	int m_targetIndex = 0;

	sf::RectangleShape m_selectionBar;
	int m_startIndex = -1;
	bool m_dragSelection = false;

public:
	TextField();
	void addChar(const char &c);
	void RemoveChar(bool withDel = false);
	void moveTarget(int dir, bool shiftPressed = false);

	bool isPointInside(float x, float y) const;
	void setSelection(bool selectionState);
	bool getSelectionState() const;

	void BeginSelection();

	void setPosition(float x, float y);
	void setSize(float x, float y);
	void setCharacterSize(float size);
	const sf::Vector2f & getSize() const;
	const sf::Vector2f & getPosition() const;

	void Press(float x, float y);

	void Update(float dt);


	const std::string & getString() const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	static bool isValidKey(const sf::Uint32 & unicode);
private:
	void _updateTextAndPosition();
};