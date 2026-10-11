#include "pch.h"
#include "AttackComponent.h"
#include "Logger.h"
#include "HealthComponent.h"
#include "TransformComponent.h"
#include <cassert>

namespace Engine
{
	AttackComponent::AttackComponent(GameObject* gameObject)
		: Component(gameObject)
	{
	}

	void AttackComponent::SetDamage(float value)
	{
		assert(value >= 0.f && "Attack damage cannot be negative");
		attackDamage = value;
	}

	void AttackComponent::SetAttackRange(float value)
	{
		assert(value >= 0.f && "Attack range cannot be negative");
		attackRange = value;
	}

	void AttackComponent::SetCooldown(float value)
	{
		assert(value >= 0.f && "Attack cooldown cannot be negative");
		attackCooldown = value;
	}

	void AttackComponent::Update(float deltaTime)
	{
		cooldownTimer += deltaTime;

		if (target == nullptr)
		{
			target = FindNearestTarget();
		}

		if (target != nullptr)
		{
			auto health = target->GetComponent<HealthComponent>();
			if (health == nullptr || !health->IsAlive())
			{
				target = FindNearestTarget();
			}
		}
	}

	void AttackComponent::Render() {}

	GameObject* AttackComponent::FindNearestTarget()
	{
		auto myTransform = gameObject->GetComponent<TransformComponent>();
		if (myTransform == nullptr)
		{
			LOG_ERROR("AttackComponent requires a TransformComponent.");
			return nullptr;
		}

		Vector2Df myPosition = myTransform->GetWorldPosition();
		GameObject* nearest = nullptr;
		float nearestDistance = 0.f;

		for (auto* other : GameWorld::Instance()->GetGameObjects())
		{
			if (other == nullptr || other == gameObject)
			{
				continue;
			}

			auto health = other->GetComponent<HealthComponent>();
			if (health == nullptr || !health->IsAlive())
			{
				continue;
			}

			auto otherTransform = other->GetComponent<TransformComponent>();
			if (otherTransform == nullptr)
			{
				continue;
			}

			float distance = (otherTransform->GetWorldPosition() - myPosition).GetLength();
			if (distance > searchRadius)
			{
				continue;
			}

			if (nearest == nullptr || distance < nearestDistance)
			{
				nearest = other;
				nearestDistance = distance;
			}
		}

		return nearest;
	}

	bool AttackComponent::IsTargetInRange() const
	{
		if (target == nullptr)
		{
			return false;
		}

		auto myTransform = gameObject->GetComponent<TransformComponent>();
		auto targetTransform = target->GetComponent<TransformComponent>();
		if (myTransform == nullptr || targetTransform == nullptr)
		{
			return false;
		}

		float distance = (targetTransform->GetWorldPosition() - myTransform->GetWorldPosition()).GetLength();
		return distance <= attackRange;
	}

	bool AttackComponent::TryAttack()
	{
		if (target == nullptr)
		{
			return false;
		}

		if (cooldownTimer < attackCooldown)
		{
			return false;
		}

		if (!IsTargetInRange())
		{
			return false;
		}

		auto health = target->GetComponent<HealthComponent>();
		if (health == nullptr || !health->IsAlive())
		{
			LOG_WARN("AttackComponent: target is not alive anymore");
			target = nullptr;
			return false;
		}

		cooldownTimer = 0.f;
		LOG_INFO(std::string("AttackComponent: attacked '") + target->GetName() + "' for " +
			std::to_string(attackDamage) + " damage");
		health->TakeDamage(attackDamage);

		if (!health->IsAlive())
		{
			target = nullptr;
		}
		return true;
	}
}
