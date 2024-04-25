#include "SPickUpTemplate.h"

void ASPickUpTemplate::Interact_Implementation(APawn* InstigatorPawn)
{
	if(!InteractPrecondition(InstigatorPawn)) return;
	
	InteractAction(InstigatorPawn);
	HideMesh();
	PreventInteraction();
	ResetPickUp();
}

// Sets default values
ASPickUpTemplate::ASPickUpTemplate()
{
	BaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
	RootComponent = BaseMeshComponent;

	DelayTime = 10.0f;
}

void ASPickUpTemplate::ResetPickUp()
{
	GetWorld()->GetTimerManager().SetTimer(ResetPickupHandle, this,
	                                       &ASPickUpTemplate::ResetPickUpDelayed, DelayTime);
}

void ASPickUpTemplate::HideMesh()
{
	BaseMeshComponent->SetVisibility(false, true);
}

void ASPickUpTemplate::ShowMesh()
{
	BaseMeshComponent->SetVisibility(true, true);
}
