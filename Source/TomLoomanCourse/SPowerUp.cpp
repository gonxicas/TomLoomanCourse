#include "SPowerUp.h"

#include "Actions/SActionComponent.h"
#include "Actions/SAction.h"


bool ASPowerUp::InteractPrecondition(APawn* InstigatorPawn) const
{
	if (!ensure(InstigatorPawn && ActionToGrant)) return false;
	
	auto ActionComponent = InstigatorPawn->FindComponentByClass<USActionComponent>();
	
	if (!ActionComponent) return false;
	
	if (ActionComponent->GetAction(ActionToGrant))
	{
		UE_LOG(LogTemp, Log, TEXT("Action: %s already known."), *GetNameSafe(ActionToGrant));
		return false;
	}
	return true;
}

void ASPowerUp::InteractAction(APawn* InstigatorPawn)
{
	
	auto ActionComponent = InstigatorPawn->FindComponentByClass<USActionComponent>();
	ActionComponent->AddAction(InstigatorPawn, ActionToGrant);
}

void ASPowerUp::PreventInteraction()
{
	SetActorEnableCollision(false);
}

void ASPowerUp::AllowInteraction()
{
	SetActorEnableCollision(true);
}

void ASPowerUp::ResetPickUpDelayed()
{
	
}
