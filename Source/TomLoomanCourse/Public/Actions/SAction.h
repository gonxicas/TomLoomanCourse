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
	
	UPROPERTY(ReplicatedUsing = "OnRep_IsRunning")
	bool bIsRunning;
	
	UFUNCTION()
	void OnRep_IsRunning();

	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetOwningComponent() const;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bIsAutoStart; 
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const {return bIsRunning;}
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator) const;
	
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	virtual UWorld* GetWorld() const override;
	
	virtual bool IsSupportedForNetworking() const override { return true;}
};
