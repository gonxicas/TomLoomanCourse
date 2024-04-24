// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAttributeComponent.h"
#include "GameFramework/Actor.h"
#include "STargetDummy.generated.h"

class USAttributeComponent;

UCLASS()
class TOMLOOMANCOURSE_API ASTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTargetDummy();

protected:
	UPROPERTY(VisibleAnywhere)
	USAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth, float Delta);

	void BeginPlay() override;
};
