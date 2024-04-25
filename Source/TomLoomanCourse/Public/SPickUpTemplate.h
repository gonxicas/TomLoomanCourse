// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPickUpTemplate.generated.h"

UCLASS(Abstract)
class TOMLOOMANCOURSE_API ASPickUpTemplate : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override final;
	ASPickUpTemplate();

protected:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BaseMeshComponent;

	UPROPERTY(EditAnywhere, Category = "PickUp")
	float DelayTime;

	UPROPERTY(EditAnywhere, Category = "PickUp")
	float HealthRestored;

	FTimerHandle ResetPickupHandle;
	
protected:
	virtual void ResetPickUp();

	virtual bool InteractPrecondition(APawn* InstigatorPawn)const {return true;}
	virtual void InteractAction(APawn* InstigatorPawn) PURE_VIRTUAL(APawn* InstigatorPawn);
	void HideMesh();
	void ShowMesh();
	virtual void PreventInteraction() PURE_VIRTUAL();
	virtual void AllowInteraction() PURE_VIRTUAL();
	virtual void ResetPickUpDelayed() PURE_VIRTUAL();

};
