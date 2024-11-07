#include "stdafx.h"
#include "Tiliemap.h"
#include "stdafx.h"
#include "Tiliemap.h"

Tiliemap::Tiliemap(const std::string& name)
	: GameObject(name)
{

}

void Tiliemap::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	UpdateTransfrom();
}

void Tiliemap::SetRotation(float angle)
{
	rotation = angle;
	UpdateTransfrom();
}

void Tiliemap::SetScale(const sf::Vector2f& s)
{
	scale = s;
	UpdateTransfrom();
}

void Tiliemap::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		sf::FloatRect rect = GetLocalBounds();
		origin.x = rect.width * ((int)preset % 3) * 0.5f;
		origin.y = rect.height * ((int)preset / 3) * 0.5f;
	}

	UpdateTransfrom();
}

void Tiliemap::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;

	UpdateTransfrom();
}


sf::FloatRect Tiliemap::GetLocalBounds() const
{
	return { 0.f, 0.f, cellCount.x * cellSize.x, cellCount.y * cellSize.y };
}

sf::FloatRect Tiliemap::GetGlobalBounds() const
{
	sf::FloatRect bounds = GetLocalBounds();
	return transfrom.transformRect(bounds);
}


void Tiliemap::Init()
{
	sortingLayer = SortingLayers::Background;
	sortingOrder = -1;

	Set({ 10, 10 }, { 50.f, 50.f });
	SetOrigin(Origins::MC);
	SetScale({ 2.f, 2.f });
}

void Tiliemap::Release()
{
}

void Tiliemap::Reset()
{
	texture = &TEXTURE_MGR.Get(spriteSheetId);

	SetOrigin(originPreset);
	SetPosition({0.f, 0.f});
}

void Tiliemap::Update(float dt)
{
}

void Tiliemap::Draw(sf::RenderWindow& window)
{
	sf::RenderStates state;
	state.texture = texture;
	state.transform = transfrom;
	window.draw(va, state);
}

void Tiliemap::Set(const sf::Vector2i& count, const sf::Vector2f& size)
{
	cellCount = count;
	cellSize = size;

	va.clear();
	va.setPrimitiveType(sf::Quads);
	va.resize(count.x * count.y * 4);

	sf::Vector2f posOffset[4] = {
		{0.f, 0.f},
		{size.x , 0.f},
		{size.x, size.y},
		{0.f, size.y},
	};

	sf::Vector2f texCoords[4] = {
		{0, 0},
		{50.f, 0},
		{50.f, 50.f},
		{0, 50.f},
	};

	for (int i = 0; i < count.y; ++i) {
		for (int j = 0; j < count.x; ++j) {
			int texIndex = Utils::RandomRange(0, 2);
			if (i == 0 || i == count.y-1 || j == 0 || j == count.x - 1) {
				texIndex = 3;
			}
			int quadIndex = i * count.x + j;
			sf::Vector2f quadPos(j * size.x, i * size.y);
			for (int k = 0; k < 4; ++k) {
				int vertexIndex = quadIndex * 4 + k;
				va[vertexIndex].position = quadPos + posOffset[k];
				va[vertexIndex].texCoords = texCoords[k];
				va[vertexIndex].texCoords.y += texIndex * 50.f;
			}
		}
	}
}

void Tiliemap::UpdateTransfrom()
{
	transfrom = sf::Transform::Identity;
	transfrom.translate(position);
	transfrom.rotate(rotation);
	transfrom.scale(scale);
	transfrom.translate(-origin);
}


