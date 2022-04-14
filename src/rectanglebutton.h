#include <SFML/Graphics.hpp>
#include <iostream>

#define SFML_NO_DEPRECATED_WARNINGS

class RecButton
{
public:
	RecButton()
	{}

	RecButton(std::string t, sf::Vector2f size, int charSize, sf::Color bgColor, sf::Color textcolor) {
		text.setString(t);
		text.setFillColor(textcolor);
		text.setCharacterSize(charSize);
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(1);
		text.setOrigin(button.getLocalBounds().width, 25.f);

		button.setSize(size);
		button.setFillColor(bgColor);
	}

	void setFont(sf::Font& font)
	{
		text.setFont(font);
	}

	void setBackColor(sf::Color color)
	{
		text.setFillColor(color);
	}

	void setTextColor(sf::Color color) {
		text.setFillColor(color);
	}

	void setPosition(sf::Vector2f pos)
	{
		button.setPosition(pos);

		float xPos = (pos.x) + 7;
		float yPos = (pos.y) - 10;
		text.setPosition({ xPos, yPos });
	}

	void drawTo(sf::RenderWindow& window)
	{
		window.draw(button);
		window.draw(text);
	}

	bool isMouseOver(sf::RenderWindow& window)
	{
		float xmp = sf::Mouse::getPosition(window).x;
		float ymp = sf::Mouse::getPosition(window).y;

		float btnPosX = button.getPosition().x;
		float btnPosY = button.getPosition().y;

		float btnxPosWidth = button.getPosition().x + button.getLocalBounds().width;
		float btnyPosHeight = button.getPosition().y + button.getLocalBounds().height;

		if (xmp < btnxPosWidth && xmp > btnPosX && ymp < btnyPosHeight && ymp > btnPosY)
		{
			return true;
		}
		return false;
	}

private:
	sf::RectangleShape button;
	sf::Text text;
};