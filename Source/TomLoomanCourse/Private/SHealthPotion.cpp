// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ASHealthPotion::ASHealthPotion()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(BaseMeshComponent);

	HealthRestored = 50.0f;
}

bool ASHealthPotion::InteractPrecondition(APawn* InstigatorPawn) const
{
	const auto AttributeComponent = USAttributeComponent::GetAttributes(InstigatorPawn);

	return AttributeComponent && !AttributeComponent->HasMaxHealth();
}

void ASHealthPotion::InteractAction(APawn* InstigatorPawn)
{
	auto AttributeComponent = USAttributeComponent::GetAttributes(InstigatorPawn);

	if(!AttributeComponent) return;
	
	AttributeComponent->ApplyHealthChange(this, HealthRestored);
	
	
}

void ASHealthPotion::PreventInteraction()
{
	SetActorEnableCollision(false);
}

void ASHealthPotion::AllowInteraction()
{
	SetActorEnableCollision(true);
}

void ASHealthPotion::ResetPickUpDelayed()
{
	ShowMesh();
	AllowInteraction();
}





