#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_ProjectileAttack.generated.h"

class UAnimMontage;
class UParticleSystem;

UCLASS()
class TOMLOOMANCOURSE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Attacks")
	TSubclassOf<AActor> ProjectileCLass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	FName HandSocketName;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimDelay;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackAnim;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UParticleSystem> CastingEffect;
	
	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);
	void AdjustSpawnRotationWithTarget(ACharacter* InstigatorCharacter,
	                                   const FVector& HandLocation, UE::Math::TRotator<double>& ProjectileRotation) const;

public:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	
	USAction_ProjectileAttack();
};
