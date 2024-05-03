#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 120;
	Health = MaxHealth;
}


void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}

bool USAttributeComponent::ApplyHealthChange(const float Delta)
{
	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta ,.0f, MaxHealth);
	
	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return ActualDelta != 0; 
}

bool USAttributeComponent::HasMaxHealth() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

bool USAttributeComponent::HealToMaxHealth()
{
	return ApplyHealthChange(MaxHealth - Health);
}

 