#include "stdafx.h"
#include "Zombie.h"
#include "Player.h"
#include "SceneGame.h"
#include "UiHud.h"

Zombie::Zombie(const std::string& name) : GameObject(name)
{
}

void Zombie::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Zombie::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Zombie::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void Zombie::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Zombie::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

sf::FloatRect Zombie::GetLocalBounds() const
{
	return body.getLocalBounds();
}

sf::FloatRect Zombie::GetGlobalBounds() const
{
	return body.getGlobalBounds();
}

void Zombie::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;

	SetType(type);
}

void Zombie::Release()
{
}

void Zombie::Reset()
{
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());

	body.setTexture(TEXTURE_MGR.Get(textureId));
	SetOrigin(Origins::MC);
	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);
	SetScale({ 1.f, 1.f });
	iszombieActive = false;
}


void Zombie::Update(float dt)
{
	if (iszombieActive == false) {
		if (player != nullptr && Utils::Distance(position, player->GetPosition()) > 10)
		{
			direction = Utils::GetNormal(player->GetPosition() - position);
			SetRotation(Utils::Angle(direction));
			SetPosition(position + direction * speed * dt);
		}
	}
	hitbox.UpdateTr(body, GetLocalBounds());

	if (hp <= 0 && sceneGame != nullptr)
	{
		if (iszombieActive == false) {
			sceneGame->OnScoreUp();
		}
		iszombieActive = true;
		DieTimer += dt;
		body.setTexture(TEXTURE_MGR.Get("graphics/blood.png"), true);
		if (DieTimer > DieDelay) {
			sceneGame->OnZombieDie(this);
			DieTimer = 0.f;
		}
	}

}

void Zombie::FixedUpdate(float dt)
{
	if (sceneGame == nullptr)
		return;

	const auto& list = sceneGame->GetZombieList();


	for (auto zombie : list)
	{
		if (!zombie->IsActive())
			continue;

		sf::FloatRect bounds = GetGlobalBounds();
		sf::FloatRect zombieBounds = zombie->GetGlobalBounds();

		if (bounds.intersects(zombieBounds))
		{
			HitBox& boxPlayer = player->GetHitBox();
			if (Utils::CheckCollision(hitbox, boxPlayer))
			{
				hitTimer += dt;
				if (hitTimer > hitDelay) {
					player->OnhitDamage(damage);
					hitTimer = 0.f;
				}
			}
			break;
		}
	}
}

void Zombie::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	hitbox.Draw(window);
}

void Zombie::SetType(Types type)
{
	this->type = type;
	switch (this->type)
	{
	case Types::Bloater:
		textureId = "graphics/bloater.png";
		maxHp = 50;
		damage = 30;
		speed = 100.f;
		break;
	case Types::Chaser:
		textureId = "graphics/chaser.png";
		maxHp = 20;
		damage = 20;
		speed = 75.f;
		break;
	case Types::Crawler:
		textureId = "graphics/crawler.png";
		maxHp = 10;
		damage = 10;
		speed = 130.f;
		break;
	}
	body.setTexture(TEXTURE_MGR.Get(textureId), true);
	hp = maxHp;
}

void Zombie::OnDamage(int d)
{
	hp -= d;
}

bool Zombie::IsZombieActive()
{
	return iszombieActive;
}


//int Zombie::GetZombieKill()
//{
//	return zombieKill;
//}