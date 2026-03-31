#include "Actions/SActionComponent.h"
#include "Actions/SAction.h"

USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<USAction> Action : DefaultActions)
	{
		AddAction(Action);
	}
}


void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, .0f, FColor::White, DebugMsg);
}

void USActionComponent::AddAction(TSubclassOf<USAction> ActionClass)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	USAction* NewAction = NewObject<USAction>(this, ActionClass);

	if (ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool USActionComponent::StartAction(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (!Action->CanStart(Instigator))
		{
			auto FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
			continue;
		}
		if (Action && Action->ActionName == ActionName)
		{
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool USActionComponent::StopAction(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}
