#pragma once
#include "GameObject.h"
class Tiliemap : public GameObject
{
#pragma once


	protected:
		sf::VertexArray va;
		std::string spriteSheetId = "graphics/background_sheet.png";
		sf::Texture* texture = nullptr;
		sf::Transform transfrom;

		sf::Vector2i cellCount;
		sf::Vector2f cellSize;

	public:
		Tiliemap(const std::string& name = "");
		~Tiliemap() = default;

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

		void Set(const sf::Vector2i& count, const sf::Vector2f& size);
		void UpdateTransfrom();
		sf::FloatRect GetLocalBounds()const override;
		sf::FloatRect GetGlobalBounds()const override;

};

