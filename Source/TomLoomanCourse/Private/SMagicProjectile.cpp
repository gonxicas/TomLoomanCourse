#include "SMagicProjectile.h"

#include "SGameplayFunctionLibrary.h"
#include "Actions/SActionComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Actions/SActionEffect.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	if (!ensure(OtherActor) || OtherActor == GetInstigator()) return;

	auto ActionComponent = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
	if (ActionComponent && ActionComponent->ActiveGameplayTags.HasTag(ParryTag))
	{
		ProjectileMovementComponent->Velocity = -ProjectileMovementComponent->Velocity;
		SetInstigator(Cast<APawn>(OtherActor));
		return;
	}
	
	if(!USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult))
	{
		return;
	}
	
	if (ActionComponent)
	{
		auto instigator = GetInstigator();
		ActionComponent->AddAction(instigator, BurningActionClass);
	}

	ActivateImpactParticleEffect();
}

void ASMagicProjectile::ActivateImpactParticleEffect()
{
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), ImpactCameraShakeBase, GetActorLocation(),
	                                       ImpactCameraShakeInnerRadius, ImpactCameraShakeOuterRadius);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticleEffect, GetActorLocation(), GetActorRotation());
	Destroy();
}

void ASMagicProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnComponentHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

	if (!ensure(OtherActor) || OtherActor == GetInstigator()) return;

	ActivateImpactParticleEffect();
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}
