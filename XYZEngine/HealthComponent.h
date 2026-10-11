#pragma once

#include "Component.h"
#include <functional>

namespace Engine
{
    class HealthComponent : public Component
    {
    public:
        HealthComponent(GameObject* gameObject);

        void Update(float deltaTime) override;
        void Render() override;

        void TakeDamage(float damage);
        void Heal(float amount);

        float GetCurrentHealth() const { return currentHealth; }
        float GetMaxHealth() const { return maxHealth; }
        float GetArmor() const { return armor; }
        float GetHealthPercent() const { return currentHealth / maxHealth; }
        bool IsAlive() const { return currentHealth > 0.0f; }

        void SetMaxHealth(float value) { maxHealth = value; }
        void SetCurrentHealth(float value) { currentHealth = value; }
        void SetArmor(float value) { armor = value; }

        std::function<void()> onDeath;

    private:
        float currentHealth;
        float maxHealth;
        float armor;
    };
}