#include "SMagicProjectile.h"

#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"


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

	if(!AttributeComponent) return;

	AttributeComponent->ApplyHealthChange(-20.f);
	Destroy();
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}
