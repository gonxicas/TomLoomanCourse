// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickUpTemplate.h"
#include "SCoinPickUp.generated.h"

class USphereComponent;

UCLASS()
class TOMLOOMANCOURSE_API ASCoinPickUp : public ASPickUpTemplate
{
	GENERATED_BODY()

public:	
	ASCoinPickUp();
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "PickUp")
	int Value;

	virtual void InteractAction(APawn* InstigatorPawn) override;
	virtual void PreventInteraction() override;
	virtual void AllowInteraction() override;
	virtual void ResetPickUpDelayed() override;
};
