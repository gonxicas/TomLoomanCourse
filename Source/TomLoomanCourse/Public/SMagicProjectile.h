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
	
	UFUNCTION()
	void OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void PostInitializeComponents() override;
};
