
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

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.f;
}

bool USAttributeComponent::ApplyHealthChange(const float Delta)
{
	Health += Delta;
	Health = FMath::Clamp(Health ,.0f, MaxHealth);
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	return true; 
}

 