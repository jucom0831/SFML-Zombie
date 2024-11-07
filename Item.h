#pragma once
#include "GameObject.h"
class Item : public GameObject

{
public:
    enum class Itemlist {
	    ammo,
	    healthpack,
    };

	static const int TotalTypes = 3;

protected:
	Itemlist Items = Itemlist::ammo;
	sf::Sprite body;
	std::string textureId;

	sf::Vector2f direction;

	int Ammo = 0;
	int heal = 0;

	DebugBox debugBox;

public:
	Item(const std::string& name = "");
	~Item() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetItem(Itemlist item);
};

