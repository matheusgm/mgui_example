#include "stdafx.h"
#include "Button.h"

gui::Button::Button(float x, float y, float width, float height,
	sf::Font* font, std::string text, unsigned character_size,
	sf::Color text_normal_color, sf::Color text_hover_color, sf::Color text_pressed_color,
	sf::Color normal_color, sf::Color hover_color, sf::Color pressed_color,
	sf::Color outline_normal_color, sf::Color outline_hover_color, sf::Color outline_pressed_color,
	short unsigned id) : BaseGui(sf::Vector2f(x, y), sf::Vector2f(width, height))
{
	this->buttonState = NORMAL;
	this->id = id;
	this->onPressedCallback = [] {};
	this->buttonPressed = false;

	// Font
	this->font = font;

	// Shape
	this->shape.setFillColor(normalColor);
	this->shape.setOutlineThickness(1.f);
	this->shape.setOutlineColor(outline_normal_color);

	// Text
	this->text.setString(text);
	if (this->font != nullptr)
		this->text.setFont(*this->font);
	this->text.setFillColor(text_normal_color);
	this->text.setCharacterSize(character_size);

	this->setSize(width, height);
	this->setPosition(x, y);

	// Colors State
	this->textNormalColor = text_normal_color;
	this->textHoverColor = text_hover_color;
	this->textPressedColor = text_pressed_color;
	this->textDisabledColor = sf::Color::Color(text_normal_color.r, text_normal_color.g, text_normal_color.b, 150);

	this->normalColor = normal_color;
	this->hoverColor = hover_color;
	this->pressedColor = pressed_color;
	this->disabledColor = sf::Color::Color(normal_color.r, normal_color.g, normal_color.b, 150);

	this->outlineNormalColor = outline_normal_color;
	this->outlineHoverColor = outline_hover_color;
	this->outlinePressedColor = outline_pressed_color;
	this->outlineDisabledColor = sf::Color::Color(outline_pressed_color.r, outline_pressed_color.g, outline_pressed_color.b, 150);

}

gui::Button::~Button()
{
}

// Acessors
const bool gui::Button::isPressed() const
{
	if (this->buttonState == PRESSED) {
		return true;
	}
	return false;
}

const std::string gui::Button::getText() const
{
	return this->text.getString();
}

const short unsigned& gui::Button::getId() const
{
	return this->id;
}

void gui::Button::setPosition(const float x, const float y)
{
	BaseGui::setPosition(x, y);

	this->shape.setPosition(this->getPosition());
	this->text.setPosition(
		this->getLeft() + (this->getWidth() / 2.f) - (this->text.getGlobalBounds().width / 2.f),
		this->getTop() + (this->getHeight() / 2.f) - (this->text.getGlobalBounds().height / 2.f) - (10.f / 2.f)
	);
}

void gui::Button::setSize(const float width, const float height)
{
	BaseGui::setSize(width, height);

	this->shape.setSize(this->getSize());
}

// Modifier
void gui::Button::setText(const std::string text)
{
	this->text.setString(text);
}

void gui::Button::setId(const short unsigned id)
{
	this->id = id;
}

void gui::Button::setDisabled(const bool disable)
{
	if (disable)
		this->buttonState = DISABLED;
	else
		this->buttonState = NORMAL;
}

void gui::Button::onPressed(std::function<void()> callback)
{
	this->onPressedCallback = callback;
}

// Functions

void gui::Button::updateEvents(sf::Event& sfEvent, const sf::Vector2f& mousePos)
{
	// Disabled
	if (this->buttonState != DISABLED) {
		// Idle
		if (!this->buttonPressed)
			this->buttonState = NORMAL;
		// Hover
		if (this->shape.getGlobalBounds().contains(mousePos)) {
			this->buttonState = HOVER;
			// Pressed
			if (sfEvent.type == sf::Event::MouseButtonPressed)
			{
				if (sfEvent.mouseButton.button == sf::Mouse::Left)
				{
					this->buttonPressed = true;
				}
			}
			if (this->buttonPressed)
				this->buttonState = PRESSED;
		}
		if (this->buttonPressed)
		{
			if (sfEvent.type == sf::Event::MouseButtonReleased)
			{
				this->buttonPressed = false;
				this->mouseReleased = true;
				if (this->shape.getGlobalBounds().contains(mousePos))
				{
					this->buttonState = HOVER;
					if (sfEvent.mouseButton.button == sf::Mouse::Left)
					{
						//this->buttonState = NORMAL;
						this->onPressedCallback();
					}
				}
			}
		}
	}
}

void gui::Button::update(const sf::Vector2f& mousePos)
{
	if (this->mouseReleased)
	{
		this->mouseReleased = false;
		this->buttonState = NORMAL;
		if (this->shape.getGlobalBounds().contains(mousePos))
			this->buttonState = HOVER;
	}

	switch (this->buttonState)
	{
	case NORMAL:
		this->shape.setFillColor(this->normalColor);
		this->text.setFillColor(this->textNormalColor);
		this->shape.setOutlineColor(this->outlineNormalColor);
		break;
	case HOVER:
		this->shape.setFillColor(this->hoverColor);
		this->text.setFillColor(this->textHoverColor);
		this->shape.setOutlineColor(this->outlineHoverColor);
		break;
	case PRESSED:
		this->shape.setFillColor(this->pressedColor);
		this->text.setFillColor(this->textPressedColor);
		this->shape.setOutlineColor(this->outlinePressedColor);
		break;
	case DISABLED:
		this->shape.setFillColor(this->disabledColor);
		this->text.setFillColor(this->textDisabledColor);
		this->shape.setOutlineColor(this->outlineDisabledColor);
		break;
	default:
		this->shape.setFillColor(sf::Color::Red);
		this->text.setFillColor(sf::Color::Blue);
		this->shape.setOutlineColor(sf::Color::Green);
		break;
	}

}

void gui::Button::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
	target.draw(this->text);
}