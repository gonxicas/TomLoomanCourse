// Fill out your copyright notice in the Description page of Project Settings.

#include "Particles/ParticleSystemComponent.h"
#include "STeleportProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASTeleportProjectile::ASTeleportProjectile()
{
	TeleportParticleEffectComponent = CreateDefaultSubobject<UParticleSystemComponent>("Teleport Particle Effect");
	TeleportParticleEffectComponent->SetupAttachment(SphereComponent);
	TeleportParticleEffectComponent->SetAutoActivate(false);
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASTeleportProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentHit.AddDynamic(this, &ASTeleportProjectile::FireParticlesElapsed);
	
	if(!GetInstigator()) return;
	
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();
	TeleportInstigator();
}

// Called every frame
void ASTeleportProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTeleportProjectile::TeleportInstigator()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandleTeleportParticles, this,
	                                       &ASTeleportProjectile::FireParticlesElapsed, .5f);
}

void ASTeleportProjectile::TeleportInstigatorElapsed()
{
	if(GetInstigator())
	{
		auto HasBeenTeleported = GetInstigator()->TeleportTo(GetActorLocation(), GetActorRotation());
	}
	Destroy();
}

void ASTeleportProjectile::FireParticlesElapsed()
{
	TeleportParticleEffectComponent->Activate();
	ProjectileMovementComponent->StopMovementImmediately();
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandleTeleport, this,
	                                       &ASTeleportProjectile::TeleportInstigatorElapsed, .2f);
}

void ASTeleportProjectile::FireParticlesElapsed(UPrimitiveComponent* HitComp, AActor* OtherActor,
									UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor == GetInstigator()) return;
	
	
	TimerHandleTeleportParticles.Invalidate();
	FireParticlesElapsed();
}
