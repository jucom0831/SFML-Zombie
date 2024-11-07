#include "stdafx.h"
#include "UiGameOver.h"

UiGameOver::UiGameOver(const std::string& name)
	: GameObject(name)
{
}

void UiGameOver::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiGameOver::SetRotation(float angle)
{
	rotation = angle;
}

void UiGameOver::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiGameOver::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiGameOver::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiGameOver::Init()
{
	
	sortingLayer = SortingLayers::UI;
	sortingOrder = 2;
}

void UiGameOver::Release()
{
}

void UiGameOver::Reset()
{
	sf::Font& font = FONT_MGR.Get("fonts/zombiecontrol.ttf");
	float textSize = 150.f;
	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();

	textGameover.setFont(font);
	textGameover.setCharacterSize(textSize);
	textGameover.setFillColor(sf::Color::White);
	Utils::SetOrigin(textGameover, Origins::MC);
	textGameover.setString("PREES ENTER\n\n TO CONTINUE");
	textGameover.setPosition(size.x * 0.3f, size.y * 0.3f);
}

void UiGameOver::Update(float dt)
{
}

void UiGameOver::Draw(sf::RenderWindow& window)
{
	window.draw(textGameover);
}
