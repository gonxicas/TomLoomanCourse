// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "SCreditSystem.h"
#include "Components/SphereComponent.h"

// Sets default values
ASHealthPotion::ASHealthPotion()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(BaseMeshComponent);

	HealthRestored = 50.0f;
	Cost = 20;
}

bool ASHealthPotion::HasEnoughCredits(APawn* InstigatorPawn) const
{
	ASCreditSystem* CreditSystem = Cast<ASCreditSystem>(InstigatorPawn->GetPlayerState());
	
	return  CreditSystem && CreditSystem->HasEnoughCredits(Cost);
}

bool ASHealthPotion::InteractPrecondition(APawn* InstigatorPawn) const
{
	const auto AttributeComponent = USAttributeComponent::GetAttributes(InstigatorPawn);

	return AttributeComponent && !AttributeComponent->HasMaxHealth() && HasEnoughCredits(InstigatorPawn);
}

void ASHealthPotion::InteractAction(APawn* InstigatorPawn)
{
	auto AttributeComponent = USAttributeComponent::GetAttributes(InstigatorPawn);

	if(!AttributeComponent) return;
	
	AttributeComponent->ApplyHealthChange(this, HealthRestored);

	ASCreditSystem* CreditSystem = Cast<ASCreditSystem>(InstigatorPawn->GetPlayerState());

	if(!CreditSystem)
	{
		return;
	}

	CreditSystem->ModifyCredits(-Cost);
	
	
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





