#include "stdafx.h"
#include "SceneGame.h"
#include "Player.h"
#include "TileMap.h"
#include "Zombie.h"
#include "Bullet.h"
#include "Item.h"
#include "UiHud.h"
#include "UiUpgrade.h"
#include "UiGameOver.h"

SceneGame::SceneGame()
	: Scene(SceneIds::Game)
{
}

void SceneGame::Init()
{
	tilemap = AddGo(new TileMap("TileMap"));
	player = AddGo(new Player("Player"));
	uiHud = AddGo(new UiHud("Ui"));
	uiUpgrade = AddGo(new UiUpgrade("UiUpgrade"));
	uiGameOver = AddGo(new UiGameOver("UiGameOver"));

	Scene::Init();
}

void SceneGame::Release()
{
	Scene::Release();
}

void SceneGame::Enter()
{

	FRAMEWORK.GetWindow().setMouseCursorVisible(false);
	cursor.setTexture(TEXTURE_MGR.Get("graphics/crosshair.png"));
	cursor.setOrigin(0.f, 0.f);

	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();
	worldView.setSize(size);
	worldView.setCenter(0.f, 0.f);

	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);

	Scene::Enter();
	zombieSpawnArea = tilemap->GetMapBounds();

	uiUpgrade->SetActive(false);
 	uiGameOver->SetActive(false);
}

void SceneGame::Exit()
{
	FRAMEWORK.GetWindow().setMouseCursorVisible(true);
	for (auto zombie : zombies)
	{
		RemoveGo(zombie);
		zombiePool.Return(zombie);
	}
	zombies.clear();

	for (auto bullet : bullets)
	{
		RemoveGo(bullet);
		bulletPool.Return(bullet);
	}
	bullets.clear();

	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	sf::Vector2f mousePos = ScreenToUi(InputMgr::GetMousePosition());

	cursor.setPosition(mousePos);

	Scene::Update(dt);


	if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneIds::Game);
	}

	if (zombies.size() == 0) {
		uiHud->Setwave(w);
		SpawnZombies(1 * w);
		w++;
	}

	uiHud->SetHp(player->GetHp(), player->GetMaxHp());
	uiHud->SetAmmo(player->GetBullet(), 30);
	uiHud->SetZombieCount(zombies.size());

	if (InputMgr::GetKeyDown(sf::Keyboard::K)) 
    {
		uiUpgrade->SetActive(!uiUpgrade->IsActive());
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Enter)) {
		FRAMEWORK.SetTimeScale(1.f);
		uiGameOver->SetActive(false);
		for (auto zombie : zombies)
		{
			RemoveGo(zombie);
			zombiePool.Return(zombie);
		}
		zombies.clear();

		for (auto item : items)
		{
			RemoveGo(item);
			itemPool.Return(item);
		}
		items.clear();
		player->Reset();
		w = 1;
		uiHud->Reset();
	}

	playTime += dt;
	if (playTime > creatTime)
	{
		SpawnItems(3);
		playTime = 0.f;
	}

	if (player != nullptr)
	{
		worldView.setCenter(player->GetPosition());
	}

}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);

	const sf::View& saveView = window.getView();
	window.setView(uiView);
	window.draw(cursor);
	window.setView(saveView);
}

void SceneGame::SpawnZombies(int count)
{
	for (int i = 0; i < count; ++i)
	{
		Zombie* zombie = zombiePool.Take();
		zombies.push_back(zombie);

		Zombie::Types zombieType = (Zombie::Types)Utils::RandomRange(0, Zombie::TotalTypes - 1);
		zombie->SetType(zombieType);

		sf::Vector2f pos = Utils::RandomPointInRect(zombieSpawnArea);
		zombie->SetPosition(pos);
		
		AddGo(zombie);
	}
}


void SceneGame::SpawnItems(int count)
{
	for (int i = 0; i < count; ++i) {
		Item* item = itemPool.Take();
		items.push_back(item);

		Item::Itemlist ItemType = (Item::Itemlist)Utils::RandomRange(0, Item::TotalTypes - 1);
		item->SetItem(ItemType);

		sf::Vector2f pos;
		pos.x = Utils::RandomRange(tilemap->GetGlobalBounds().left, tilemap->GetGlobalBounds().left + tilemap->GetGlobalBounds().width);
		pos.y = Utils::RandomRange(tilemap->GetGlobalBounds().top, tilemap->GetGlobalBounds().top + tilemap->GetGlobalBounds().height);
		item->SetPosition(pos);

		AddGo(item);
	}
}

Bullet* SceneGame::TakeBullet()
{
	Bullet* bullet = bulletPool.Take();
	bullets.push_back(bullet);
	AddGo(bullet);
	return bullet;
}

void SceneGame::ReturnBullet(Bullet* bullet)
{
	RemoveGo(bullet);
	bulletPool.Return(bullet);
	bullets.remove(bullet);
}

void SceneGame::OnZombieDie(Zombie* zombie)
{
	RemoveGo(zombie);
	zombiePool.Return(zombie);
	zombies.remove(zombie);
}

void SceneGame::OnUpgrade(Upgrade u)
{
	uiUpgrade->SetActive(!uiUpgrade->IsActive());

}

void SceneGame::OnPlayerDie(Player* player) {
	uiGameOver->SetActive(true);
	FRAMEWORK.SetTimeScale(0.f);
}

void SceneGame::OnScoreUp()
{
	score += 100;
	uiHud->SetScore(score);
}

void SceneGame::OnGaugeHpDown()
{
	
}
