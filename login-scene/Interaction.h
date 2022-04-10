#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Textbox {
public:
	sf::Text text;
	sf::RectangleShape box;

	Textbox(const std::string &_text, const sf::Font& font, const int &char_size, const sf::Color& char_color,
		const sf::Vector2f& position, const sf::Vector2f& box_size, const sf::Color& box_color,
		const sf::Vector2f& off_set = sf::Vector2f(0, 0)) {
		text.setString(_text);
		text.setFont(font);
		text.setCharacterSize(char_size);
		text.setFillColor(char_color);
		box.setPosition(position);
		box.setSize(box_size);
		box.setFillColor(box_color);
		text.setPosition(position + off_set);
	}

	Textbox(){}

	void set_text(const std::string& _text);
	void set_text_style(const sf::Uint32& char_style);
	void set_outline(const sf::Color& color, const float& thickness = 2);
	void set_box_color(const sf::Color& color);
	void draw(sf::RenderWindow& window);
	bool inside(const float& x, const float& y);
};

class Button {
public:
	float left, right, up, down;
	bool idle = true;
	bool use_image = false;

	sf::Image idle_image, hover_image;
	Textbox idle_textbox, hover_textbox;

	Button(const Textbox& _idle_textbox, const Textbox& _hover_textbox)
	{
		left = _idle_textbox.box.getPosition().x - _idle_textbox.box.getOrigin().x;
		up = _idle_textbox.box.getPosition().y - _idle_textbox.box.getOrigin().y;
		right = left + _idle_textbox.box.getSize().x;
		down = up + _idle_textbox.box.getSize().y;
		idle_textbox = _idle_textbox;
		hover_textbox = _hover_textbox;
	}

	void draw(sf::RenderWindow& window);
	bool inside(const float& x, const float& y);

	void (*trigger) (float x, float y);
};

class Interaction {
public:
	std::vector<Button> buttons; // implement using vector for now, change to list later

	void add_button(const Button& button);

	void interact(const sf::Event& event);

	void draw(sf::RenderWindow& window);
};