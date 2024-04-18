// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"


// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	FHitResult Hit;
	FVector End;

	FVector EyeLocation;
	FRotator EyeRotation;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	auto MyOwner = GetOwner();
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	End = EyeLocation + (EyeRotation.Vector() * 1000);
	GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	auto HitActor = Hit.GetActor();
	if(!HitActor || !HitActor->Implements<USGameplayInterface>()) return;

	ISGameplayInterface::Execute_Interact(HitActor, Cast<APawn>(MyOwner));
}
