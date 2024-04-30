// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if(!ensure(BlackboardComponent)) return;

	AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActor"));
	if(!TargetActor) return;

	auto MyController = OwnerComp.GetAIOwner();
	if(!ensure(MyController)) return;

	auto AIPawn = MyController->GetPawn();
	if(!ensure(AIPawn)) return;
	
	float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
	auto bWithinRange = DistanceTo < 2000.0f;
	
	auto bHasLineOfSight = bWithinRange ?  MyController->LineOfSightTo(TargetActor) : false;
	
	BlackboardComponent->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && bHasLineOfSight);
	
}
