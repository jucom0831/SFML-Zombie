#pragma once
#include "Scene.h"
#include <UiUpgrade.h>

class Player;
class Zombie;
class Bullet;
class Map;
class TileMap;
class Item;
class UiHud;
class UiUpgrade;
class UiGameOver;

class SceneGame : public Scene
{
protected:
	TileMap* tilemap;
	Player* player;
	UiHud* uiHud;
	UiUpgrade* uiUpgrade;
	UiGameOver* uiGameOver;
	Zombie* zombie;

	std::list<Zombie*> zombies;
	ObjectPool<Zombie> zombiePool;
	sf::FloatRect zombieSpawnArea;

	std::list<Bullet*> bullets;
	ObjectPool<Bullet> bulletPool;

	std::list<Item*> items;
	ObjectPool<Item> itemPool;

	float playTime = 0.f;
	float creatTime = 10.f;

	sf::Sprite cursor;

	int w = 1;
	int score = 0;

public:
	SceneGame();
	virtual ~SceneGame() = default;

	void Init();
	void Release();
	void Enter();
	void Exit();
	void Update(float dt);
	void Draw(sf::RenderWindow& window) override;

	void SpawnItems(int count);
	void SpawnZombies(int count);
	Bullet* TakeBullet();
	void ReturnBullet(Bullet* bullet);

	const std::list<Zombie*>& GetZombieList() const { return zombies; }

	void OnZombieDie(Zombie* zombie);

	void OnUpgrade(Upgrade u);

	void OnPlayerDie(Player* player);
	
	void OnScoreUp();
	void OnGaugeHpDown();
};

