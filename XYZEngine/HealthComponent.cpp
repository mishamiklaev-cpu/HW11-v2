#include "pch.h"
#include "HealthComponent.h"
#include "Logger.h"
#include <cassert>

namespace Engine
{
    HealthComponent::HealthComponent(GameObject* gameObject)
        : Component(gameObject)
        , currentHealth(100.0f)
        , maxHealth(100.0f)
        , armor(0.0f)
    {
    }

    void HealthComponent::Update(float deltaTime)
    {
        // Пусто, но нужно для Component
    }

    void HealthComponent::Render()
    {
        // Пусто, но нужно для Component
    }

    void HealthComponent::TakeDamage(float damage)
    {
        assert(damage >= 0.0f && "Damage cannot be negative");

        if (!IsAlive())
        {
            LOG_WARN("HealthComponent: Already dead, ignoring damage");
            return;
        }

        float actualDamage = damage - armor;
        if (actualDamage < 0.0f)
        {
            actualDamage = 0.0f;
        }

        currentHealth -= actualDamage;

        if (currentHealth < 0.0f)
        {
            currentHealth = 0.0f;
        }

        LOG_WARN("HealthComponent took " + std::to_string(actualDamage) +
            " damage. HP: " + std::to_string(currentHealth) + "/" + std::to_string(maxHealth));

        if (!IsAlive())
        {
            LOG_INFO("HealthComponent: Entity died!");
            if (onDeath)
            {
                onDeath();
            }
        }
    }

    void HealthComponent::Heal(float amount)
    {
        assert(amount >= 0.0f && "Heal amount cannot be negative");

        if (!IsAlive())
        {
            LOG_WARN("HealthComponent: Cannot heal dead entity");
            return;
        }

        currentHealth += amount;
        if (currentHealth > maxHealth)
        {
            currentHealth = maxHealth;
        }

        LOG_INFO("HealthComponent healed for " + std::to_string(amount) +
            ". HP: " + std::to_string(currentHealth) + "/" + std::to_string(maxHealth));
    }
}