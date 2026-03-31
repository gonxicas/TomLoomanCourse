#include "Actions/SAction.h"

#include "Actions/SActionComponent.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	
	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantedTags);
	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	
	ensureAlways(bIsRunning);
	
	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantedTags);
	bIsRunning = false;
}


bool USAction::CanStart_Implementation(AActor* Instigator) const
{
	if (bIsRunning) return false;
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

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}