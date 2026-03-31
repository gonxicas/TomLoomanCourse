#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "UObject/Object.h"
#include "SActionEffect.generated.h"

UCLASS()
class TOMLOOMANCOURSE_API USActionEffect : public USAction
{
	GENERATED_BODY()
	
public:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;
	
	USActionEffect();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;
	
	FTimerHandle DurationHandle;
	FTimerHandle PeriodHandle;
	
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);
	
	
};
