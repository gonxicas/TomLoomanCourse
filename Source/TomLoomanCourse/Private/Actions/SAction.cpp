#include "Actions/SAction.h"

#include "Actions/SActionComponent.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
	
	GetActionComponent()->ActiveGameplayTags.AppendTags(GrantedTags);
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
	
	GetActionComponent()->ActiveGameplayTags.RemoveTags(GrantedTags);
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

USActionComponent* USAction::GetActionComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}
