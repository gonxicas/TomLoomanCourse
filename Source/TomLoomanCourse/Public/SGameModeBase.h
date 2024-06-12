#pragma once

#include "CoreMinimal.h"
#include "SCharacter.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"


class ASPickUpTemplate;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
/**
 * 
 */
UCLASS()
class TOMLOOMANCOURSE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASGameModeBase();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyFloat;

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	UPROPERTY(EditAnywhere, Category = "Game Mode")
	float SpawnDelay;

	UPROPERTY(EditAnywhere, Category = "Credits")
	int CreditsPerKill = 10;

	UPROPERTY(EditAnywhere, Category = "PickUp")
	UEnvQuery* PickUpSpawnQuery;

	UPROPERTY(EditAnywhere, Category = "PickUp")
	TArray<TSubclassOf<ASPickUpTemplate>> PickUpClasses;

	UPROPERTY(EditAnywhere, Category = "PickUp")
	int NumberOfPickUps = 3;

	UPROPERTY(EditAnywhere, Category = "PickUp")
	float MinimumDistanceAmongPickUps = 300.f;
	
	virtual void StartPlay() override;
	bool HasReachedMaximumBotCapacity();

	UFUNCTION()
	void SpawnBotTimerElapsed();
	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void OnPowerUpQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);
	void RespawnPlayer(AActor* VictimActor, AActor* Killer, ASCharacter* Player);
	void GiveCreditsToPlayer(const ASCharacter* Player) const;

public:
	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	UFUNCTION(Exec)
	void KillAll();
};
