#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SSaveGame.generated.h"

UCLASS()
class TOMLOOMANCOURSE_API USSaveGame : public USaveGame
{
	GENERATED_BODY()
	
	public:
	UPROPERTY()
	int32 Credits;
};
