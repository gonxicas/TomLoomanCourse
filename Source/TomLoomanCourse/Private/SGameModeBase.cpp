#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SCreditSystem.h"
#include "SPickUpTemplate.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true,
                                                TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed,
	                                SpawnTimerInterval, true);
	if (!ensure(PickUpSpawnQuery) || !ensure(PickUpClasses.Num() > 0))
	{
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* PickUpQueryInstance = UEnvQueryManager::RunEQSQuery(
		this, PickUpSpawnQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
	if (!PickUpQueryInstance)
	{
		return;
	}

	PickUpQueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::ASGameModeBase::OnPowerUpQueryCompleted);
}

bool ASGameModeBase::HasReachedMaximumBotCapacity()
{
	int32 NumberOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(Bot);
		if (AttributeComponent && AttributeComponent->IsAlive())
		{
			NumberOfAliveBots++;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %i alive bots."), NumberOfAliveBots);
	float MaxBotCount = 10.f;
	if (DifficultyFloat)
	{
		MaxBotCount = DifficultyFloat->GetFloatValue(GetWorld()->TimeSeconds);
	}
	if (NumberOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity. Skipping bot spawn."));
	}
	return NumberOfAliveBots >= MaxBotCount;
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}
	if (HasReachedMaximumBotCapacity())
	{
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(
		this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct,
		nullptr);

	if (!ensure(QueryInstance))
	{
		return;
	}
	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                      EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}


	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

	if (!Locations.IsValidIndex(0))
	{
		DrawDebugSphere(GetWorld(), Locations[0], 50.0f, 20, FColor::Blue, false, 60.0f);
		return;
	}

	GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
}

void ASGameModeBase::OnPowerUpQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                             EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("PickUP EQS Query Failed!"));
		return;
	}
	TArray<FVector> Locations;
	QueryInstance->GetQueryResultsAsLocations(Locations);

	TArray<FVector> UsedLocations;

	if (Locations.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("PickUP EQS Query returned 0 locations!"));
		return;
	}

	int NumberOfPickUpSpawned = 0;
	while (Locations.Num() > 0 && NumberOfPickUpSpawned < NumberOfPickUps)
	{
		int LocationIndex = FMath::RandRange(0, Locations.Num() - 1);
		FVector CurrentLocation = Locations[LocationIndex];
		Locations.RemoveAt(LocationIndex);

		bool bIsValidLocation = true;
		for (FVector UsedLocation : UsedLocations)
		{
			float Distance = (UsedLocation - CurrentLocation).Size();
			
			if(Distance < MinimumDistanceAmongPickUps)
			{
				bIsValidLocation = false;
				break;
			}
		}
		
		if(!bIsValidLocation)
		{
			continue;
		}

		int PickUpIndex = FMath::RandRange(0, PickUpClasses.Num() - 1);

		TSubclassOf<ASPickUpTemplate> PickUp = PickUpClasses[PickUpIndex];

		GetWorld()->SpawnActor<ASPickUpTemplate>(PickUp, CurrentLocation, FRotator::ZeroRotator);
		UsedLocations.Add(CurrentLocation);
		NumberOfPickUpSpawned++;
	}
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (!ensure(Controller))
	{
		return;
	}

	Controller->UnPossess();

	RestartPlayer(Controller);
}

void ASGameModeBase::RespawnPlayer(AActor* VictimActor, AActor* Killer, ASCharacter* Player)
{
	FTimerHandle TimerHandle_RespawnDelay;

	FTimerDelegate Delegate;
	Delegate.BindUObject(this, &ASGameModeBase::RespawnPlayerElapsed, Player->GetController());


	SpawnDelay = 2.0f;
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, SpawnDelay, false);

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor),
	       *GetNameSafe(Killer));
}

void ASGameModeBase::GiveCreditsToPlayer(const ASCharacter* Player) const
{
	ASCreditSystem* CreditSystem = Cast<ASCreditSystem>(Player->GetPlayerState());
	if (!CreditSystem)
	{
		return;
	}
	CreditSystem->ModifyCredits(CreditsPerKill);
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if (Player)
	{
		RespawnPlayer(VictimActor, Killer, Player);
		return;
	}

	ASAICharacter* Bot = Cast<ASAICharacter>(VictimActor);
	Player = Cast<ASCharacter>(Killer);
	if (Bot && Player)
	{
		GiveCreditsToPlayer(Player);
	}
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributes(Bot);
		if (AttributeComponent && AttributeComponent->IsAlive())
		{
			AttributeComponent->Kill(this);
		}
	}
}
