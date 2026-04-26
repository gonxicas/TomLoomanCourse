#pragma once

#include "CoreMinimal.h"
#include "SCharacter.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"


class USMonsterData;
class USSaveGame;
class ASPickUpTemplate;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class UDataTable;


USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()
	
	FMonsterInfoRow() : Weight(1.f), SpawnCost(5.f), KillReward(20.f) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USMonsterData> MonsterData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward;
};

UCLASS()
class TOMLOOMANCOURSE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASGameModeBase();

protected:
	
	FString SlotName;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UDataTable> MonsterDataTable;
	
	UPROPERTY()
	TObjectPtr<USSaveGame> CurrentSaveGame;
	
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
	virtual void StartPlay() override;
	
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	UFUNCTION(Exec)
	void KillAll();
	
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();
	
	void LoadSaveGame();
};
