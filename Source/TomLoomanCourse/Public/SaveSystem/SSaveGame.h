#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SSaveGame.generated.h"

USTRUCT()
struct FActorSavedData
{
	GENERATED_BODY()

	UPROPERTY()
	FString ActorName;
	
	UPROPERTY()
	FTransform Transform;
};

UCLASS()
class TOMLOOMANCOURSE_API USSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 Credits;
	
	UPROPERTY()
	TArray<FActorSavedData> SavedActors;
};
