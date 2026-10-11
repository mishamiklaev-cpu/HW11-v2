#include "AI.h"
#include "ResourceSystem.h"
#include "Logger.h"
#include "TransformComponent.h"

namespace Roguelike
{
    AI::AI(const Engine::Vector2Df& position, Engine::GameObject* player)
    {
        gameObject = Engine::GameWorld::Instance()->CreateGameObject("AI");
        auto transform = gameObject->GetComponent<Engine::TransformComponent>();
        transform->SetWorldPosition(position);

        auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
        renderer->SetTexture(*Engine::ResourceSystem::Instance()->GetTextureMapElementShared("AI", 0));
        renderer->SetPixelSize(100, 100);

        auto rigidbody = gameObject->AddComponent<Engine::RigidbodyComponent>();
        rigidbody->SetKinematic(false);

        auto follower = gameObject->AddComponent<Engine::FollowComponent>();
        follower->SetTarget(player);
        follower->SetSpeed(120.0f);
        follower->SetDetectionRadius(350.0f);

        healthComponent = gameObject->AddComponent<Engine::HealthComponent>();
        healthComponent->SetMaxHealth(50.0f);
        healthComponent->SetCurrentHealth(50.0f);
        healthComponent->SetArmor(5.0f);

        healthComponent->onDeath = [this]() {
            LOG_INFO("AI enemy died!");
            isAlive = false;
            Engine::GameWorld::Instance()->DestroyGameObject(gameObject);
            };

        auto collider = gameObject->AddComponent<Engine::SpriteColliderComponent>();

        attackComponent = gameObject->AddComponent<Engine::AttackComponent>();
        attackComponent->SetDamage(15.0f);
        attackComponent->SetAttackRange(120.0f);
        attackComponent->SetCooldown(1.2f);

        LOG_INFO("AI enemy created successfully. HP: " + std::to_string(healthComponent->GetCurrentHealth()) +
            ", Armor: " + std::to_string(healthComponent->GetArmor()) +
            ", Damage: " + std::to_string(attackComponent->GetDamage()) +
            ", Cooldown: " + std::to_string(attackComponent->GetCooldown()));
    }

    Engine::GameObject* AI::GetGameObject()
    {
        return gameObject;
    }

    Engine::HealthComponent* AI::GetHealthComponent()
    {
        return healthComponent;
    }

    Engine::AttackComponent* AI::GetAttackComponent()
    {
        return attackComponent;
    }

    void AI::Update(float deltaTime)
    {
        if (isFlashing)
        {
            flashTimer -= deltaTime;
            if (flashTimer <= 0.0f)
            {
                isFlashing = false;
            }
        }

        if (attackComponent != nullptr)
        {
            attackComponent->TryAttack();
        }
    }

    void AI::UpdateUI(sf::RenderWindow& window)
    {
        if (!isAlive || healthComponent == nullptr)
        {
            return;
        }

        if (!uiInitialized)
        {
            if (font.loadFromFile("assets/Fonts/Roboto-Regular.ttf"))
            {
                hpText.setFont(font);
                hpText.setCharacterSize(18);
                hpText.setFillColor(sf::Color::Red);
                uiInitialized = true;
            }
            else
            {
                LOG_ERROR("Failed to load font for AI UI!");
                return;
            }
        }

        auto transform = gameObject->GetComponent<Engine::TransformComponent>();
        auto worldPos = transform->GetWorldPosition();

        std::string hpString = "HP: " + std::to_string((int)healthComponent->GetCurrentHealth()) + "/" + std::to_string((int)healthComponent->GetMaxHealth());
        hpText.setString(hpString);

        if (isFlashing)
        {
            hpText.setFillColor(sf::Color::Yellow);
        }
        else
        {
            hpText.setFillColor(sf::Color::Red);
        }

        sf::View originalView = window.getView();
        sf::Vector2f worldPosition(worldPos.x, worldPos.y);
        sf::Vector2i screenPosition = window.mapCoordsToPixel(worldPosition);

        window.setView(window.getDefaultView());

        float textWidth = hpText.getLocalBounds().width;
        hpText.setOrigin(textWidth / 2.0f, 0.0f);

        hpText.setPosition((float)screenPosition.x, (float)screenPosition.y - 80.0f);

        window.draw(hpText);

        window.setView(originalView);
    }
}