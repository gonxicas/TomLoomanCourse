#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "SAttributeComponent.h"
#include "SActionEffect_Thorn.generated.h"

UCLASS()
class TOMLOOMANCOURSE_API USActionEffect_Thorn : public USActionEffect
{
	GENERATED_BODY()
	
public:
	USActionEffect_Thorn();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect")
	float ReflectPercentage;
	
};
