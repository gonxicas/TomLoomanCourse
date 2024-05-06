#include "SAttributeComponent.h"

#include "SGameModeBase.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f,
                                                        TEXT("Global Damage Modifier for Attribute Component."),
                                                        ECVF_Cheat);

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

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	
	if (!GetOwner()->CanBeDamaged() && Delta < .0f)
	{
		return false;
	}

	if(Delta < .0f)
	{
		Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}
	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, .0f, MaxHealth);

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta);

	if (ActualDelta < .0f && Health == .0f)
	{
		ASGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASGameModeBase>();

		if (GameMode)
		{
			GameMode->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return ActualDelta != 0;
}

bool USAttributeComponent::HasMaxHealth() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

bool USAttributeComponent::HealToMaxHealth(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, MaxHealth - Health);
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -MaxHealth);
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (!FromActor)
	{
		return nullptr;
	}

	return FromActor->FindComponentByClass<USAttributeComponent>();
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComponent = GetAttributes(Actor);
	if (!AttributeComponent)
	{
		return false;
	}

	return AttributeComponent->IsAlive();
}
