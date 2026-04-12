#include "SAnimInstance.h"

#include "GameplayTagContainer.h"
#include "Actions/SActionComponent.h"

void USAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (const auto OwningActor = GetOwningActor())
	{
		ActionComponent = Cast<USActionComponent>(OwningActor->GetComponentByClass(USActionComponent::StaticClass()));
	}
}

void USAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	static FGameplayTag StunnedTag = FGameplayTag::RequestGameplayTag("Status.Stunned");
	if (ActionComponent)
	{
		bIsStunned = ActionComponent->ActiveGameplayTags.HasTag(StunnedTag);
	}
}
