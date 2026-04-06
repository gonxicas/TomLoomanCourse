#include "Actions/SAttributeComponent.h"

#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f,
                                                        TEXT("Global Damage Modifier for Attribute Component."),
                                                        ECVF_Cheat);

USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 120;
	Health = MaxHealth;
	Rage = 0;
	MaxRage = 150;

	SetIsReplicatedByDefault(true);
}

void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void USAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < .0f)
	{
		return false;
	}

	if (Delta < .0f)
	{
		Delta *= CVarDamageMultiplier.GetValueOnGameThread();
	}
	float OldHealth = Health;
	Health = FMath::Clamp(Health + Delta, .0f, MaxHealth);

	float ActualDelta = Health - OldHealth;
	if (ActualDelta != 0.f)
	{
		MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
	}

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

void USAttributeComponent::ApplyRageChange(float RageAmount)
{
	Rage = FMath::Clamp(Rage + RageAmount, .0f, MaxRage);
	MulticastRageChanged(GetOwner(), Rage, RageAmount);
}

bool USAttributeComponent::TryUseRage(float RageCost)
{
	const auto bCanAfford = CanAffordRage(RageCost);
	if (bCanAfford)
	{
		ApplyRageChange(-RageCost);
	}
	return bCanAfford;
}

bool USAttributeComponent::CanAffordRage(float RageCost)
{
	return Rage >= RageCost;
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

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, MaxHealth);
	DOREPLIFETIME(USAttributeComponent, Rage);
	DOREPLIFETIME(USAttributeComponent, MaxRage);
	//DOREPLIFETIME_CONDITION(USAttributeComponent, MaxHealth, COND_OwnerOnly)
}
