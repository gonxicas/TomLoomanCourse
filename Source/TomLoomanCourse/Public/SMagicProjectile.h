#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

UCLASS()
class TOMLOOMANCOURSE_API ASMagicProjectile : public ASProjectile
{
	GENERATED_BODY()

public:
	ASMagicProjectile();

protected:
	UPROPERTY(EditAnywhere, Category = "Damage")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* ImpactParticleEffect;

	

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* ImpactSound;

	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void ActivateImpactParticleEffect();

	virtual void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                            FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void PostInitializeComponents() override;
};
