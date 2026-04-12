#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class UInputAction;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, APlayerState*, NewPlayerState);

UCLASS()
class TOMLOOMANCOURSE_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChanged OnPlayerStateReceived;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> PauseMenuInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> InputToggleMenu;

	virtual void SetupInputComponent() override;
	
	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();
	
	
	
public:
	virtual void OnRep_PlayerState() override;
};
