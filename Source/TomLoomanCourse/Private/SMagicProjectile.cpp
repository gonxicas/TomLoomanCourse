#include "SMagicProjectile.h"

#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	if (!ensure(OtherActor) || OtherActor == GetInstigator()) return;

	auto AttributeComponent = Cast<USAttributeComponent>(
		OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));

	if (!AttributeComponent) return;

	AttributeComponent->ApplyHealthChange(GetInstigator(), -Damage);
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
