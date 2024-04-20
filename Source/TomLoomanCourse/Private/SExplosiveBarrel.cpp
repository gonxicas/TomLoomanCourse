// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include "Components/CapsuleComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMeshComponent->SetSimulatePhysics(true);

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("Radial Force");
	RadialForceComponent->SetupAttachment(StaticMeshComponent);
	RadialForceComponent->Radius = 600;
	RadialForceComponent->ImpulseStrength = 2000;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::FireImpulse);
}

void ASExplosiveBarrel::FireImpulse(UPrimitiveComponent* HitComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComponent->FireImpulse();

	UE_LOG(LogTemp, Log, TEXT("OnActorHit in Explosive Barrel"))

	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time: %f"),
	       *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

	FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString,
	                nullptr, FColor::Green, 2.f, true);
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
