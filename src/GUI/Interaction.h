#pragma once

#include <SFML/Graphics.hpp>
#include "../App/List.h"
#include "../Constants.h"
#include "../App/App.h"

class Outline {
public:
	sf::Color color = sf::Color::White;
	float thickness = 0;

	Outline(const sf::Color& _color, const float& _thickness) : color(_color), thickness(_thickness) {}
	Outline() = default;
};

class Textbox {
private:
	sf::Font font_tmp; //for getting the bound of the text, kinda dump ngl

public:
	sf::Text text;
	sf::RectangleShape box;

	Textbox(const std::string &_text, const int &char_size, const sf::Color& char_color,
		const sf::Vector2f& position, const sf::Vector2f& box_size, const sf::Color& box_color) {
		font_tmp.loadFromFile(PATH::DEFAULT_FONT);
		text.setFont(font_tmp);
		text.setString(_text);
		text.setCharacterSize(char_size);
		text.setFillColor(char_color);
		box.setPosition(position);
		box.setSize(box_size);
		box.setFillColor(box_color);
		align_center();
	}

	Textbox() = default;

	void set_text(const std::string& _text);
	void set_text_size(const int& char_size);
	void set_text_color(const sf::Color& char_color);
	void set_text_style(const sf::Uint32& char_style);

	void align_left(const float& offset = 5);
	void align_center();
	void align_right(const float& offset = 5);

	void set_box_position(const sf::Vector2f& position);
	void set_box_origin(const sf::Vector2f& origin);
	void set_box_size(const sf::Vector2f& box_size);
	void set_box_color(const sf::Color& color);

	void set_outline(const sf::Color& color, const float& thickness = -2);
	void set_outline(const Outline &outline);

	void draw(sf::RenderWindow& window, sf::Font& font);
	bool inside(const int& x, const int& y);
};

class Input_Textbox {
public:
	Textbox textbox;
	std::string text;
	int length_limit = 1000;
	Outline idle_outline, selected_outline;
	bool idle = true;
	float align_offset = 5;
	bool hide_text = false;

	Input_Textbox(const Textbox& _textbox, const int& _length_limit, const sf::Color& selected_outline_color,
		const float& selected_outline_thickness = -2) {
		textbox = _textbox;
		set_selected_outline(selected_outline_color, selected_outline_thickness);
		length_limit = _length_limit;
	}

	Input_Textbox() = default;

	void set_lenth_limit(int limit);
	void set_idle_outline(const sf::Color& color, const float& thickness = -2);
	void set_selected_outline(const sf::Color& color, const float& thickness = -2);
	void set_align_offset(const float& offset);
	void set_hide_text(const bool& hide = true);
	void add_char(char c);
	void pop_char();
	void draw(sf::RenderWindow& window, sf::Font& font);
	bool inside(const int& x, const int& y);
};

class Button_Textbox {
public:
	Textbox textbox;
	Outline idle_outline, hover_outline;
	bool idle = true;

	Button_Textbox(const Textbox& _textbox, const sf::Color& hover_outline_color, const float& hover_outline_thickness = -2) {
		textbox = _textbox;
		set_hover_outline(hover_outline_color, hover_outline_thickness);
	}

	Button_Textbox() = default;

	void set_idle_outline(const sf::Color& color, const float& thickness = -2);
	void set_hover_outline(const sf::Color& color, const float& thickness = -2);
	void draw(sf::RenderWindow& window, sf::Font& font);
	bool inside(const int& x, const int& y);
};

class Button_Sprite {
public:
	sf::Sprite idle_sprite, hover_sprite;
	sf::Texture texture;
	bool idle = true;

	Button_Sprite(const sf::Texture& _texture, const sf::Vector2f& position, const sf::Vector2f& size,
		const sf::Color& hover_color = sf::Color::Cyan) {
		texture = _texture;
		idle_sprite.setTexture(texture);
		idle_sprite.setPosition(position);
		idle_sprite.setScale(size.x / idle_sprite.getGlobalBounds().width, size.y / idle_sprite.getGlobalBounds().height);
		hover_sprite = idle_sprite;
		hover_sprite.setColor(hover_color);
	}
	Button_Sprite() = default;

	void draw(sf::RenderWindow& window, sf::Font& font);
	bool inside(const int& x, const int& y);
};

template <class T>
class Button_List {
public:
	List<T> list_button;
	std::function<void(int)> trigger;

	void add_button(const T& button);
	void draw(sf::RenderWindow& window, sf::Font& font);
	void update_hover(const int& x, const int& y);
	void update_trigger(const int& x, const int& y);
};

class Interaction {
public:
	List<Input_Textbox*> list_inptb;
	List<Button_List<Button_Textbox>> type1_buttons;
	List<Button_List<Button_Sprite>> type2_buttons;
	Input_Textbox* selected_textbox = NULL;

	void add_input_textbox(Input_Textbox& inptb);
	void add_button_list(const Button_List<Button_Textbox>& button_list);
	void add_button_list(const Button_List<Button_Sprite>& button_list);
	void add_button(const Button_Textbox& button, const std::function<void(int)>& trigger);
	void add_button(const Button_Sprite& button, const std::function<void(int)>& trigger);
	void interact(sf::RenderWindow& window);
	void draw(sf::RenderWindow& window, sf::Font& font);
};
