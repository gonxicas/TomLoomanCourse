#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SCreditSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnCreditsChanged, float, NewCredits, float, Delta);

UCLASS()
class TOMLOOMANCOURSE_API ASCreditSystem : public APlayerState
{
	GENERATED_BODY()

public:
	ASCreditSystem();

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Credits")
	int CurrentCredits;

public:
	
	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;

	UFUNCTION(BlueprintCallable)
	int GetCurrentCredits() const {return CurrentCredits;}

	UFUNCTION(BlueprintCallable)
	bool ModifyCredits(float CreditsToAdd);
};
