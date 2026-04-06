#include "Actions/SAction.h"

#include "Actions/SActionComponent.h"
#include "Net/UnrealNetwork.h"
#include "TomLoomanCourse/TomLoomanCourse.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	//UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);
	
	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantedTags);
	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	// UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	LogOnScreen(this,FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);
	
	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantedTags);
	bIsRunning = false;
}


bool USAction::CanStart_Implementation(AActor* Instigator) const
{
	if (bIsRunning)
	{
		return false;
	}
	return !GetOwningComponent()->ActiveGameplayTags.HasAny(BlockedTags);
}

UWorld* USAction::GetWorld() const
{
	UActorComponent* Component = Cast<UActorComponent>(GetOuter());
	if (Component)
	{
		return Component->GetWorld();
	}
	return nullptr;
}

void USAction::OnRep_IsRunning()
{
	if (bIsRunning)
	{
		StartAction(nullptr);
	}
	else
	{
		StopAction(nullptr);
	}
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}

void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(USAction, bIsRunning);
}