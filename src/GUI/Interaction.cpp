#include "Interaction.h"

void Textbox::set_text(const std::string& _text) {
	text.setString(_text);
	align_center();
}

void Textbox::set_text_size(const int& char_size) {
	text.setCharacterSize(char_size);
}

void Textbox::set_text_color(const sf::Color& char_color) {
	text.setFillColor(char_color);
}

void Textbox::set_text_style(const sf::Uint32& char_style) {
	text.setStyle(char_style);
}

void Textbox::align_left(const float& offset) {
	text.setOrigin(0, text.getGlobalBounds().height / 2);
	sf::Rect<float> box_bounds = box.getGlobalBounds();
	text.setPosition(box_bounds.left + offset, box_bounds.top + box_bounds.height / 2);
}

void Textbox::align_center() {
	text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
	sf::Rect<float> box_bounds = box.getGlobalBounds();
	text.setPosition(box_bounds.left + box_bounds.width / 2, box_bounds.top + box_bounds.height / 2);
}

void Textbox::align_right(const float& offset) {
	text.setOrigin(text.getGlobalBounds().width, text.getGlobalBounds().height / 2);
	sf::Rect<float> box_bounds = box.getGlobalBounds();
	text.setPosition(box_bounds.left + box_bounds.width - offset, box_bounds.top + box_bounds.height / 2);
}

void Textbox::set_box_position(const sf::Vector2f& position) {
	box.setPosition(position);
	align_center();
}

void Textbox::set_box_origin(const sf::Vector2f& origin) {
	box.setOrigin(origin);
}

void Textbox::set_box_size(const sf::Vector2f& box_size) {
	box.setSize(box_size);
}

void Textbox::set_box_color(const sf::Color& color) {
	box.setFillColor(color);
}

void Textbox::set_outline(const sf::Color& color, const float& thickness) {
	box.setOutlineColor(color);
	box.setOutlineThickness(thickness);
}

void Textbox::set_outline(const Outline &outline) {
	box.setOutlineColor(outline.color);
	box.setOutlineThickness(outline.thickness);
}

void Textbox::draw(sf::RenderWindow& window, sf::Font& font) {
	window.draw(box);
	text.setFont(font);
	window.draw(text);
}

bool Textbox::inside(const int& x, const int& y) {
	float left, right, up, down;
	left = box.getPosition().x - box.getOrigin().x;
	up = box.getPosition().y - box.getOrigin().y;
	right = left + box.getSize().x;
	down = up + box.getSize().y;
	return (left <= x && x < right && up <= y && y < down);
}

void Input_Textbox::set_lenth_limit(int limit) {
	length_limit = limit;
}

void Input_Textbox::set_idle_outline(const sf::Color& color, const float& thickness) {
	idle_outline = Outline(color, thickness);
}

void Input_Textbox::set_selected_outline(const sf::Color& color, const float& thickness) {
	selected_outline = Outline(color, thickness);
}

void Input_Textbox::set_align_offset(const float& offset) {
	align_offset = offset;
}

void Input_Textbox::set_hide_text(const bool& hide) {
	hide_text = hide;
}

void Input_Textbox::add_char(char c) {
	if (text.length() == length_limit)
		return;
	text += c;
}

void Input_Textbox::pop_char() {
	if (text.empty())
		return;
	text.pop_back();
}

void Input_Textbox::draw(sf::RenderWindow& window, sf::Font& font) {
	if (hide_text) {
		std::string replace_text(text.size(), '*');
		textbox.set_text(replace_text);
	}
	else
		textbox.set_text(text);
	textbox.align_left(align_offset);
	if (idle)
		textbox.set_outline(idle_outline);
	else
		textbox.set_outline(selected_outline);
	textbox.draw(window, font);
}

bool Input_Textbox::inside(const int& x, const int& y) {
	return textbox.inside(x, y);
}

void Button_Textbox::set_idle_outline(const sf::Color& color, const float& thickness) {
	idle_outline = { color, thickness };
}

void Button_Textbox::set_hover_outline(const sf::Color& color, const float& thickness) {
	hover_outline = { color, thickness };
}

void Button_Textbox::draw(sf::RenderWindow& window, sf::Font& font) {
	if (idle)
		textbox.set_outline(idle_outline);
	else
		textbox.set_outline(hover_outline);
	textbox.draw(window, font);
}

bool Button_Textbox::inside(const int& x, const int& y) {
	return textbox.inside(x, y);
}

void Button_Sprite::draw(sf::RenderWindow& window, sf::Font& font) {
	if (idle)
		window.draw(idle_sprite);
	else
		window.draw(hover_sprite);
}

bool Button_Sprite::inside(const int& x, const int& y) {
	float left, right, up, down;
	left = idle_sprite.getPosition().x - idle_sprite.getOrigin().x;
	up = idle_sprite.getPosition().y - idle_sprite.getOrigin().y;
	right = left + idle_sprite.getGlobalBounds().width;
	down = up + idle_sprite.getGlobalBounds().height;
	return (left <= x && x < right && up <= y && y < down);
}

template <class T>
void Button_List<T>::add_button(const T& button) {
	list_button.push_back(button);
}

template <class T>
void Button_List<T>::draw(sf::RenderWindow& window, sf::Font& font) {
	list_button.for_each([&](T& button) {
		button.draw(window, font);
	});
}

template <class T>
void Button_List<T>::update_hover(const int& x, const int& y) {
	list_button.for_each([&](T& button) {
		button.idle = !button.inside(x, y);
	});
}

template <class T>
void Button_List<T>::update_trigger(const int& x, const int& y) {
	int i = 0;
	list_button.for_each([&](T& button) {
		if (button.inside(x, y))
			trigger(i);
		i++;
	});
}

void Interaction::add_input_textbox(Input_Textbox& inptb) {
	list_inptb.push_back(&inptb);
}

void Interaction::add_button_list(const Button_List<Button_Textbox>& button_list) {
	type1_buttons.push_back(button_list);
}

void Interaction::add_button_list(const Button_List<Button_Sprite>& button_list) {
	type2_buttons.push_back(button_list);
}

void Interaction::add_button(const Button_Textbox& button, const std::function<void(int)>& trigger) {
	Button_List<Button_Textbox> button_list;
	button_list.add_button(button);
	button_list.trigger = trigger;
	type1_buttons.push_back(button_list);
}

void Interaction::add_button(const Button_Sprite& button, const std::function<void(int)>& trigger) {
	Button_List<Button_Sprite> button_list;
	button_list.add_button(button);
	button_list.trigger = trigger;
	type2_buttons.push_back(button_list);
}

void Interaction::interact(sf::RenderWindow& window) {
	sf::Event event;
	window.waitEvent(event);
	if (event.type == sf::Event::Closed) {
		window.close();
	}
	else if (event.type == sf::Event::MouseMoved) {
		int x = event.mouseMove.x;
		int y = event.mouseMove.y;
		type1_buttons.for_each([&](Button_List<Button_Textbox>& button_list) {
			button_list.update_hover(x, y);
		});
		type2_buttons.for_each([&](Button_List<Button_Sprite>& button_list) {
			button_list.update_hover(x, y);
		});
	}
	else if (event.type == sf::Event::MouseButtonPressed) {
		int x = event.mouseButton.x;
		int y = event.mouseButton.y;
		if (event.mouseButton.button == sf::Mouse::Left) {
			type1_buttons.for_each([&](Button_List<Button_Textbox>& button_list) {
				button_list.update_trigger(x, y);
			});
			type2_buttons.for_each([&](Button_List<Button_Sprite>& button_list) {
				button_list.update_trigger(x, y);
			});
			selected_textbox = NULL;
			list_inptb.for_each([&](Input_Textbox*& inptb) {
				if (inptb->inside(x, y)) {
					inptb->idle = false;
					selected_textbox = inptb;
				}
				else
					inptb->idle = true;
			});
		}
	}
	else if (event.type == sf::Event::TextEntered) {
		if (selected_textbox == NULL) return;
		char c = static_cast<char>(event.text.unicode);
		if (c == 8)
			selected_textbox->pop_char();
		else if (32 <= c && c <= 126) // printable characters
			selected_textbox->add_char(c);
	}
}

void Interaction::draw(sf::RenderWindow& window, sf::Font& font) {
	type1_buttons.for_each([&](Button_List<Button_Textbox>& button_list) {
		button_list.draw(window, font);
	});
	type2_buttons.for_each([&](Button_List<Button_Sprite>& button_list) {
		button_list.draw(window, font);
	});
	list_inptb.for_each([&](Input_Textbox*& inptb) {
		inptb->draw(window, font);
	});
}
