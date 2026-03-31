#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class USActionComponent;
class UWorld;

UCLASS(Blueprintable)
class TOMLOOMANCOURSE_API USAction : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer GrantedTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer BlockedTags;

	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetActionComponent() const;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	virtual UWorld* GetWorld() const override;
};
