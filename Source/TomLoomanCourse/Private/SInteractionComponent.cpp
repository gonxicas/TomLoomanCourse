// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteractions(TEXT("su.DrawDebugInteraction"), false,
                                                            TEXT("Enable debug lines for Interact Component."),
                                                            ECVF_Cheat);

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

	bool bDebugDraw = CVarDebugDrawInteractions.GetValueOnGameThread();
	
	// FHitResult Hit;
	FVector End;

	FVector EyeLocation;
	FRotator EyeRotation;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	auto MyOwner = GetOwner();
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	End = EyeLocation + (EyeRotation.Vector() * 1000);
	// auto blockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	auto Radius = 30.f;
	Shape.SetSphere(Radius);
	auto blockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams,
	                                                      Shape);


	FColor Color = blockingHit ? FColor::Green : FColor::Red;
	for (FHitResult Hit : Hits)
	{
		auto HitActor = Hit.GetActor();
		if(bDebugDraw)
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, Color, false, 2.f);

		if (!HitActor || !HitActor->Implements<USGameplayInterface>()) continue;

		ISGameplayInterface::Execute_Interact(HitActor, Cast<APawn>(MyOwner));
		blockingHit = true;
		break;
	}
	if(bDebugDraw)
		DrawDebugLine(GetWorld(), EyeLocation, End, Color,
	              false, 2.f, 0, 2.f);
}
