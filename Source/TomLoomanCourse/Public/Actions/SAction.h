#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SAction.generated.h"

class USActionComponent;
class UWorld;
class UTexture2D;

USTRUCT()
struct FActionWrappedData
{
	GENERATED_BODY();

	UPROPERTY()
	bool bIsRunning;
	UPROPERTY()
	TObjectPtr<AActor> Instigator;
};
UCLASS(Blueprintable)
class TOMLOOMANCOURSE_API USAction : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer GrantedTags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer BlockedTags;
	
	UPROPERTY(ReplicatedUsing = "OnRep_RepData")
	FActionWrappedData RepData;
	
	float TimeStarted;
	
	UFUNCTION()
	void OnRep_RepData();

	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetOwningComponent() const;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bIsAutoStart; 
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const {return RepData.bIsRunning;}
	
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
