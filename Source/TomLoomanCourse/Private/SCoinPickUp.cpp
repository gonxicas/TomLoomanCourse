// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoinPickUp.h"

#include "SCreditSystem.h"
#include "Components/SphereComponent.h"

ASCoinPickUp::ASCoinPickUp() : Value(50)
{
}

void ASCoinPickUp::InteractAction(APawn* InstigatorPawn)
{
	ASCreditSystem* CreditSystem = Cast<ASCreditSystem>(InstigatorPawn->GetPlayerState());

	if(!CreditSystem)
	{
		return;
	}

	CreditSystem->ModifyCredits(Value);
}

void ASCoinPickUp::PreventInteraction()
{
	SetActorEnableCollision(false);
}

void ASCoinPickUp::AllowInteraction()
{
	SetActorEnableCollision(true);
}

void ASCoinPickUp::ResetPickUpDelayed()
{
	ShowMesh();
	AllowInteraction();
}


