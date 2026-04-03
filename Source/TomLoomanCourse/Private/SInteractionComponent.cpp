#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDebugDrawInteractions(TEXT("su.DrawDebugInteraction"), false,
                                                            TEXT("Enable debug lines for Interact Component."),
                                                            ECVF_Cheat);

USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	TraceDistance = 500.f;
	TraceRadius = 30.f;
	CollisionChannel = ECC_WorldDynamic;
}


void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}


void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	auto MyPawn = Cast<APawn>(GetOwner());
	if (!MyPawn->IsLocallyControlled()) return;
	
	FindBestInteractable();
}

void USInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw = CVarDebugDrawInteractions.GetValueOnGameThread();

	FVector End;

	FVector EyeLocation;
	FRotator EyeRotation;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	auto MyOwner = GetOwner();
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	End = EyeLocation + (EyeRotation.Vector() * TraceDistance);
	// auto blockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	auto blockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity,
	                                                      ObjectQueryParams, Shape);


	FocusedActor = nullptr;
	FColor Color = blockingHit ? FColor::Green : FColor::Red;
	for (FHitResult Hit : Hits)
	{
		auto HitActor = Hit.GetActor();
		if (bDebugDraw)
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, Color, false, 2.f);

		if (!HitActor || !HitActor->Implements<USGameplayInterface>()) continue;

		FocusedActor = HitActor;
		break;
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->SetAttachedActor(FocusedActor);
			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
	if (bDebugDraw)
		DrawDebugLine(GetWorld(), EyeLocation, End, Color,
		              false, 0.f, 0, 2.f);
}

void USInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}

void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "No Focus Actor to Interact with");
		return;
	}


	ISGameplayInterface::Execute_Interact(InFocus, Cast<APawn>(GetOwner()));
}
