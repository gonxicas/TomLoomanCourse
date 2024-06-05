// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickUpTemplate.h"
#include "GameFramework/Actor.h"
#include "SHealthPotion.generated.h"

class USphereComponent;

UCLASS()
class TOMLOOMANCOURSE_API ASHealthPotion : public ASPickUpTemplate
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASHealthPotion();
	bool HasEnoughCredits(APawn* InstigatorPawn) const;

protected:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, Category = "PickUp")
	float HealthRestored;

	UPROPERTY(EditAnywhere, Category = "PickUp")
	int Cost;

	
	
protected:
	// Called when the game starts or when spawned
	virtual bool InteractPrecondition(APawn* InstigatorPawn)const override;
	virtual void InteractAction(APawn* InstigatorPawn) override;
	virtual void PreventInteraction() override;
	virtual void AllowInteraction() override;
	virtual void ResetPickUpDelayed() override;

};
