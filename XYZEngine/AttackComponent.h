#pragma once

#include "Component.h"
#include "GameWorld.h"

namespace Engine
{
	class TransformComponent;
	class HealthComponent;

	class AttackComponent : public Component
	{
	public:
		AttackComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetDamage(float value);
		void SetAttackRange(float value);
		void SetCooldown(float value);
		void SetTarget(GameObject* targetObject) { target = targetObject; }
		void SetSearchRadius(float value) { searchRadius = value; }

		float GetDamage() const { return attackDamage; }
		float GetCooldown() const { return attackCooldown; }

		GameObject* FindNearestTarget();
		bool TryAttack();

	private:
		float attackDamage = 10.f;
		float attackRange = 100.f;
		float attackCooldown = 1.f;
		float cooldownTimer = 0.f;
		float searchRadius = 300.f;

		GameObject* target = nullptr;

		bool IsTargetInRange() const;
	};
}
