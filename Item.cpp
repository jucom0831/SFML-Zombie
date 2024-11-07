#include "stdafx.h"
#include "Item.h"

Item::Item(const std::string& name)
	: GameObject(name)
{
}

void Item::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Item::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Item::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void Item::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = (Utils::SetOrigin(body, preset));
	}
}

void Item::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void Item::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 1;

	SetItem(Items);
}

void Item::Release()
{
}

void Item::Reset()
{
	body.setTexture(TEXTURE_MGR.Get(textureId), true);
	SetOrigin(Origins::MC);
	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);
	SetScale({ 1.f, 1.f });
}

void Item::Update(float dt)
{
	debugBox.SetBounds(body.getGlobalBounds());

}

void Item::Draw(sf::RenderWindow& window)
{
	window.draw(body);	
	debugBox.Draw(window);
}

void Item::SetItem(Itemlist item)
{
	this->Items = item;
	switch (this->Items) {
	case Itemlist::ammo:
		textureId = "graphics/ammo_icon.png";
		break;
	case Itemlist::healthpack:
		textureId = "graphics/health_pickup.png";
		break;
	}

	body.setTexture(TEXTURE_MGR.Get(textureId), true);
}
