#pragma once

class Player;
class SceneGame;
class UiHud;

class Zombie : public GameObject
{

public:
	enum class Types
	{
		Bloater,
		Chaser,
		Crawler,
	};

	static const int TotalTypes = 3;

protected:
	Types type = Types::Bloater;
	sf::Sprite body;
	std::string textureId;

	sf::Vector2f direction;

	UiHud* uiHud;

	int maxHp = 0;
	float speed = 0.f;
	int damage = 0;
	float attackInterval = 0.f;

	int hp = 0;
	float attackTimer = 0.f;

	Player* player;
	SceneGame* sceneGame = nullptr;

	float hitDelay = 2.f;
	float hitTimer = 0.f;

	float DieDelay = 3.f;
	float DieTimer = 0.f;

	bool iszombieActive = false;

public:
	Zombie(const std::string& name = "");
	~Zombie() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	sf::FloatRect GetLocalBounds() const override;
	sf::FloatRect GetGlobalBounds() const override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetType(Types type);

	void OnDamage(int damge);

	bool IsZombieActive();
};
