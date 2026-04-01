#include "Actions/SActionEffect_Thorn.h"

#include "SGameplayFunctionLibrary.h"
#include "Actions/SActionComponent.h"
#include "Actions/SAttributeComponent.h"

USActionEffect_Thorn::USActionEffect_Thorn()
{
	Duration = .0f;
	Period = .0f;
	ReflectPercentage = 0.5f;
}

void USActionEffect_Thorn::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	auto OwningActor = GetOwningComponent()->GetOwner();
	if (Delta >= .0f || OwningActor == InstigatorActor) return;
	
	auto ReflectDamage = FMath::RoundToInt(Delta * ReflectPercentage);
	if (ReflectDamage == 0) return;
	
	
	USGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, FMath::Abs(ReflectDamage));
	
}

void USActionEffect_Thorn::StartAction_Implementation(AActor* Instigator)
{
	auto Attributes = USAttributeComponent::GetAttributes(Instigator);
	if (Attributes)
	{
		Attributes->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorn::OnHealthChanged);
	}
	Super::StartAction_Implementation(Instigator);
}

void USActionEffect_Thorn::StopAction_Implementation(AActor* Instigator)
{
	auto Attributes = USAttributeComponent::GetAttributes(Instigator);
	if (Attributes)
	{
		Attributes->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorn::OnHealthChanged);
	}
	Super::StopAction_Implementation(Instigator);
}
