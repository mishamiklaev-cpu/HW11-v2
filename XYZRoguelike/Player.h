#pragma once

#include "CameraComponent.h"
#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "RenderSystem.h"
#include "InputComponent.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "AttackComponent.h"
#include <cassert>
#include <SFML/Graphics.hpp>

namespace Roguelike
{
	class Player
	{
	public:
		Player();
		Engine::GameObject* GetGameObject();
		Engine::HealthComponent* GetHealthComponent();
		Engine::AttackComponent* GetAttackComponent();

		bool Attack(Engine::GameObject* targetObject);

		void UpdateUI(sf::RenderWindow& window);

	private:
		Engine::GameObject* gameObject;
		Engine::HealthComponent* healthComponent;
		Engine::AttackComponent* attackComponent;

		sf::Font font;
		sf::Text hpText;
		bool uiInitialized = false;
	};
}