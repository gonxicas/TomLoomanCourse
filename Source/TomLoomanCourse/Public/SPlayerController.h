#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, APlayerState*, NewPlayerState);

UCLASS()
class TOMLOOMANCOURSE_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChanged OnPlayerStateRecieved;
	
public:
	virtual void OnRep_PlayerState() override;
};
