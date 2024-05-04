#include "AI/SBTTask_RangedAttack.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

USBTTask_RangedAttack::USBTTask_RangedAttack()
{
	MaxBulletSpreadVertical = 2.0f;
	MaxBulletSpreadHorizontal = 2.0f;
}

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();

	if(!ensure(MyController))
	{
		return EBTNodeResult::Failed;
	}

	ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());

	if(!MyPawn)
	{
		return EBTNodeResult::Failed;
	}

	FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");

	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));

	if(!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	if(	!USAttributeComponent::IsActorAlive(TargetActor))
	{
		return EBTNodeResult::Failed;
	}

	FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
	FRotator MuzzleRotation = Direction.Rotation();

	MuzzleRotation.Pitch += FMath::RandRange(.0f, MaxBulletSpreadHorizontal);
	MuzzleRotation.Yaw += FMath::RandRange(.0f, MaxBulletSpreadVertical);
	
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Instigator = MyPawn;
	AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);

	return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;

	
}