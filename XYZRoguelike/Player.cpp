#include "Player.h"
#include "AI.h"
#include <ResourceSystem.h>
#include <SpriteColliderComponent.h>
#include "Application.h"
#include "PlayerMovementComponent.h"
#include "Logger.h"

namespace Roguelike
{
    Player::Player()
    {
        gameObject = Engine::GameWorld::Instance()->CreateGameObject("Player");
        auto transform = gameObject->GetComponent<Engine::TransformComponent>();
        transform->SetWorldPosition(Engine::Vector2Df{ 960.0f, 540.0f });

        auto playerRenderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
        const auto* texture = Engine::ResourceSystem::Instance()->GetTextureShared("ball");
        if (texture)
        {
            playerRenderer->SetTexture(*texture);
        }
        else
        {
            LOG_ERROR("Failed to load player texture 'ball'!");
        }

        playerRenderer->SetPixelSize(64, 64);

        auto playerCamera = gameObject->AddComponent<Engine::CameraComponent>();
        playerCamera->SetWindow(&Application::Instance().GetWindow());
        playerCamera->SetBaseResolution(1280, 720);

        auto playerInput = gameObject->AddComponent<Engine::InputComponent>();
        auto body = gameObject->AddComponent<Engine::RigidbodyComponent>();
        body->SetLinearDamping(15.0f);

        auto movement = gameObject->AddComponent<Engine::PlayerMovementComponent>();
        movement->SetSpeed(200.0f);

        healthComponent = gameObject->AddComponent<Engine::HealthComponent>();
        healthComponent->SetMaxHealth(100.0f);
        healthComponent->SetCurrentHealth(100.0f);
        healthComponent->SetArmor(10.0f);

        healthComponent->onDeath = [this]() {
            LOG_ERROR("Player died!");
            Application::Instance().GetGame().LooseGame();
            };

        auto collider = gameObject->AddComponent<Engine::SpriteColliderComponent>();

        attackComponent = gameObject->AddComponent<Engine::AttackComponent>();
        attackComponent->SetDamage(25.0f);
        attackComponent->SetAttackRange(150.0f);
        attackComponent->SetCooldown(0.5f);

        LOG_INFO("Player created successfully. HP: " + std::to_string(healthComponent->GetCurrentHealth()) + ", Armor: " + std::to_string(healthComponent->GetArmor()) +
            ", Damage: " + std::to_string(attackComponent->GetDamage()));
    }

    Engine::GameObject* Player::GetGameObject()
    {
        return gameObject;
    }

    Engine::HealthComponent* Player::GetHealthComponent()
    {
        return healthComponent;
    }

    Engine::AttackComponent* Player::GetAttackComponent()
    {
        return attackComponent;
    }

    bool Player::Attack(Engine::GameObject* targetObject)
    {
        if (attackComponent == nullptr || targetObject == nullptr)
        {
            LOG_WARN("Player attack failed: no attack component or no target");
            return false;
        }

        attackComponent->SetTarget(targetObject);
        bool isHit = attackComponent->TryAttack();

        if (!isHit)
        {
            LOG_INFO("Player attack missed: target out of range or attack is recharging");
        }

        attackComponent->SetTarget(nullptr);
        return isHit;
    }

    void Player::UpdateUI(sf::RenderWindow& window)
    {
        if (!uiInitialized)
        {
            if (font.loadFromFile("assets/Fonts/Roboto-Regular.ttf"))
            {
                hpText.setFont(font);
                hpText.setCharacterSize(24);
                hpText.setFillColor(sf::Color::White);
                uiInitialized = true;
                LOG_INFO("Player UI initialized");
            }
            else
            {
                LOG_ERROR("Failed to load font for Player UI!");
                return;
            }
        }

        std::string hpString = "HP: " + std::to_string((int)healthComponent->GetCurrentHealth()) + "/" + std::to_string((int)healthComponent->GetMaxHealth()) +
            " | Armor: " + std::to_string((int)healthComponent->GetArmor());
        hpText.setString(hpString);

        sf::View originalView = window.getView();
        sf::View screenView(sf::FloatRect(0.f, 0.f, 1280.f, 720.f));
        window.setView(screenView);

        hpText.setPosition(20.f, 20.f);
        window.draw(hpText);

        window.setView(originalView);
    }
}