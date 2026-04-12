#include "Actions/SAction.h"

#include "Actions/SActionComponent.h"
#include "Net/UnrealNetwork.h"
#include "TomLoomanCourse/TomLoomanCourse.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	// LogOnScreen(this, FString::Printf(TEXT("Started: %s"), *ActionName.ToString()), FColor::Green);
	
	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantedTags);
	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
	
	TimeStarted = GetWorld()->TimeSeconds;
	
	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	// LogOnScreen(this,FString::Printf(TEXT("Stopped: %s"), *ActionName.ToString()), FColor::White);
	
	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantedTags);
	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
	
	GetOwningComponent()->OnActionStopped.Broadcast(GetOwningComponent(), this);
}


bool USAction::CanStart_Implementation(AActor* Instigator) const
{
	if (RepData.bIsRunning)
	{
		return false;
	}
	return !GetOwningComponent()->ActiveGameplayTags.HasAny(BlockedTags);
}

UWorld* USAction::GetWorld() const
{
	if (const UActorComponent* Component = Cast<UActorComponent>(GetOuter()))
	{
		return Component->GetWorld();
	}
	return nullptr;
}

void USAction::OnRep_RepData()
{
	if (RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}

void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(USAction, RepData);
}