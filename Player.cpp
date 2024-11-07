#include "stdafx.h"
#include "Player.h"
#include "SceneGame.h"
#include "Bullet.h"
#include "Zombie.h"
#include "UiHud.h"

Player::Player(const std::string& name)
	: GameObject(name)
{

}

void Player::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Player::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(angle);
}

void Player::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(s);
}

int Player::GetHp()
{
	return Hp;
}

int Player::GetMaxHp()
{
	return Maxhp;
}

int Player::GetBullet()
{
	return bullet;
}

void Player::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Player::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void Player::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
	SetOrigin(Origins::MC);
	isplayerActive = false;
}

void Player::Release()
{
}

void Player::Reset()
{
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());

	body.setTexture(TEXTURE_MGR.Get(textureId), true);
	SetOrigin(originPreset);
	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);
	direction = { 1.f, 0.f };

	shootTimer = shootDelay;
	isplayerActive = false;

	SetBullet(30);
}

void Player::Update(float dt)
{
	if (isplayerActive == false) {
		direction.x = InputMgr::GetAxis(Axis::Horizontal);
		direction.y = InputMgr::GetAxis(Axis::Vertical);
		float mag = Utils::Magnitude(direction);
		if (mag > 1.f)
		{
			Utils::Normailize(direction);
		}

		sf::Vector2i mousePos = InputMgr::GetMousePosition();
		sf::Vector2f mouseWorldPos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(mousePos);
		look = Utils::GetNormal(mouseWorldPos - position);

		SetRotation(Utils::Angle(look));
		SetPosition(position + direction * speed * dt);

		shootTimer += dt;
		if (bullet != 0) {
			if (shootTimer > shootDelay && InputMgr::GetMouseButton(sf::Mouse::Left))
			{
				shootTimer = 0.f;
				bullet--;
				Shoot();
			}
		}
		if (InputMgr::GetKeyDown(sf::Keyboard::R)) {
			SetBullet(30);
		}
	}
	hitbox.UpdateTr(body, GetLocalBounds());
}


void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}

void Player::Shoot()
{
	Bullet* bullet = sceneGame->TakeBullet();
	bullet->Fire(position, look, 1000.f, 10);
}

void Player::OnhitDamage(int d)
{
	Hp -= d;
	if (Hp <= 0 && sceneGame != nullptr)
	{
		isplayerActive = true;
		sceneGame->OnPlayerDie(this);
	}
}

int Player::SetBullet(int b)
{
	bullet = b;
	return bullet;
}

sf::FloatRect Player::GetLocalBounds() const
{
	return body.getLocalBounds();
}

sf::FloatRect Player::GetGlobalBounds() const
{
	return body.getGlobalBounds();
}