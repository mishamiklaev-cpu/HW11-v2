#pragma once

#include "GameWorld.h"
#include "GameObject.h"
#include "Vector.h"
#include "FollowComponent.h"
#include "RigidbodyComponent.h"
#include "SpriteColliderComponent.h"
#include "HealthComponent.h"
#include "AttackComponent.h"
#include <cassert>
#include <SFML/Graphics.hpp>

namespace Roguelike
{
	class AI
	{
	public:
		AI(const Engine::Vector2Df& position, Engine::GameObject* player);
		Engine::GameObject* GetGameObject();
		Engine::HealthComponent* GetHealthComponent();
		Engine::AttackComponent* GetAttackComponent();

		bool IsAlive() const { return isAlive; }

		void UpdateUI(sf::RenderWindow& window);
		void Update(float deltaTime);

	private:
		Engine::GameObject* gameObject;
		Engine::HealthComponent* healthComponent;
		Engine::AttackComponent* attackComponent;

		sf::Font font;
		sf::Text hpText;
		bool uiInitialized = false;

		float flashTimer = 0.0f;
		bool isFlashing = false;
		bool isAlive = true;
	};
}