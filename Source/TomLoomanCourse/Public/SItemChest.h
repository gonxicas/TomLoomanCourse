// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class TOMLOOMANCOURSE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	void Interact_Implementation(APawn* InstigatorPawn);
	ASItemChest();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(ReplicatedUsing="OnRep_LidOpened", BlueprintReadOnly, SaveGame)//RepNotify
	bool bLidOpened;
	
	UPROPERTY(EditAnywhere)
	float TargetPitch;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;

	UFUNCTION()
	void OnRep_LidOpened();
public:
	virtual void OnActorLoaded_Implementation() override;
	
	virtual void OnActorLoaded_Implementation() override;
	
	
	
};
