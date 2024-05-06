// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"

#include "SAttributeComponent.h"

static TAutoConsoleVariable<float> CVarDamageImpulseMultiplier(
	TEXT("su.DamageImpulse"), 1.f, TEXT("Damage impulse modifier added when a character dies."), ECVF_Cheat);

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComponent)
	{
		return AttributeComponent->ApplyHealthChange(DamageCauser, -DamageAmount);
	}

	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
                                                       const FHitResult& HitResult)
{
	if (!ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		return false;
	}

	UPrimitiveComponent* HitComponent = HitResult.GetComponent();
	if (HitComponent && HitComponent->IsSimulatingPhysics())
	{
		float ImpulseModifier = CVarDamageImpulseMultiplier.GetValueOnGameThread();
		HitComponent->AddImpulseAtLocation(-HitResult.ImpactNormal * 300000.f * ImpulseModifier, HitResult.ImpactPoint,
	                                   HitResult.BoneName);
	}
	return true;
}
