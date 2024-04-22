// Fill out your copyright notice in the Description page of Project Settings.

#include "STeleportProjectile.h"
#include "Particles/ParticleSystemComponent.h"
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

	TeleportParticleEffectTime = .5f;
	TeleportTime = .2f;
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
	TeleportParticleEffectTime = .5f;
	GetWorld()->GetTimerManager().SetTimer(TimerHandleTeleportParticles, this,
	                                       &ASTeleportProjectile::FireParticlesElapsed, TeleportParticleEffectTime);
}

void ASTeleportProjectile::TeleportInstigatorElapsed()
{
	if(GetInstigator())
	{
		auto HasBeenTeleported = GetInstigator()->TeleportTo(GetActorLocation(), GetInstigator()->GetActorRotation());
	}
	Destroy();
}

void ASTeleportProjectile::FireParticlesElapsed()
{
	ParticleEffectComponent->DeactivateSystem();
	
	TeleportParticleEffectComponent->Activate();
	ProjectileMovementComponent->StopMovementImmediately();

	TeleportTime = .2f;
	GetWorld()->GetTimerManager().SetTimer(TimerHandleTeleport, this,
	                                       &ASTeleportProjectile::TeleportInstigatorElapsed, TeleportTime);
}

void ASTeleportProjectile::FireParticlesElapsed(UPrimitiveComponent* HitComp, AActor* OtherActor,
									UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor == GetInstigator()) return;
	
	
	GetWorldTimerManager().ClearTimer(TimerHandleTeleportParticles);
	FireParticlesElapsed();
}
