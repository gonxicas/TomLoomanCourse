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

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing="OnRep_ModifyCredits", Category = "Credits")
	int CurrentCredits;

public:
	
	UPROPERTY(BlueprintAssignable)
	FOnCreditsChanged OnCreditsChanged;

	UFUNCTION(BlueprintCallable)
	int GetCurrentCredits() const {return CurrentCredits;}

	UFUNCTION(BlueprintCallable, Unreliable, Server)
	void Server_ModifyCredits(int CreditsToAdd);
	
	UFUNCTION(BlueprintCallable)
	void OnRep_ModifyCredits(int CreditsToAdd) const;

	UFUNCTION(BlueprintCallable)
	bool HasEnoughCredits(int CreditsToSpend) const;
};
