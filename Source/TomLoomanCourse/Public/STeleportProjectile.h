// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "GameFramework/Actor.h"
#include "STeleportProjectile.generated.h"

UCLASS()
class TOMLOOMANCOURSE_API ASTeleportProjectile : public ASProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTeleportProjectile();

protected:
	FTimerHandle TimerHandleTeleport;
	FTimerHandle TimerHandleTeleportParticles;

	UPROPERTY(VisibleAnywhere, Category = "Particles")
	UParticleSystemComponent* TeleportParticleEffectComponent;
	
	// Called when the game starts or when spawned
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void TeleportInstigator();
	void TeleportInstigatorElapsed();
	void FireParticlesElapsed();
	UFUNCTION()
	void FireParticlesElapsed(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                          FVector NormalImpulse, const FHitResult& Hit);
};
