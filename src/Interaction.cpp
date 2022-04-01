#include "Interaction.h"

void Textbox::set_text_style(const sf::Uint32 &char_style) {
	text.setStyle(char_style);
}

void Textbox::set_outline(const sf::Color &color, const float &thickness) {
	box.setOutlineColor(color);
	box.setOutlineThickness(thickness);
}

void Textbox::draw(sf::RenderWindow &window) {
	window.draw(box);
	window.draw(text);
}

void Button::draw(sf::RenderWindow &window) {
	if (use_image) {
		if (idle) {

		}
		else {

		}
	}
	else {
		if (idle) {
			idle_textbox.draw(window);
		}
		else {
			hover_textbox.draw(window);
		}
	}
}

bool Button::inside(const float &x, const float &y) {
	return (left <= x && x < right && up <= y && y < down);
}

void Interaction::add_button(const Button &button) {
	buttons.push_back(button);
}

void Interaction::interact(const sf::Event &event) { // check if it is mouse moved or press first?
	if (event.type == sf::Event::MouseMoved) {
		float x = event.mouseMove.x;
		float y = event.mouseMove.y;
		
		for (Button &button : buttons) {
			button.idle = !button.inside(x, y);
		}
	}
	else {
		float x = event.mouseButton.x;
		float y = event.mouseButton.y;

		for (Button button : buttons) {
			if (button.inside(x, y)) {
				button.trigger(x, y);
				break;
			}
		}
	}
}

void Interaction::draw(sf::RenderWindow &window) {
	for (Button button : buttons) {
		button.draw(window);
	}
}