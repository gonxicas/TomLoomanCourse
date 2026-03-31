#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"

class USAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOMLOOMANCOURSE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer ActiveGameplayTags;
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	void AddAction(AActor* Instigator, TSubclassOf<USAction>  ActionClass);
	
	UFUNCTION(BlueprintCallable, Category = "Action")
	void RemoveAction(USAction*  ActionToRemove);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StartAction(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool StopAction(AActor* Instigator, FName ActionName);
	
	USActionComponent();

protected:
	
	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<TSubclassOf<USAction>> DefaultActions;
	
	UPROPERTY()
	TArray<USAction*> Actions;
	
	
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
