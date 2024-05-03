// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_ReactToPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USBTTask_ReactToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (!ensure(MyController))
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BlackboardComponent = Cast<UBlackboardComponent>(MyController->GetBlackboardComponent());

	if (!BlackboardComponent)
	{
		return EBTNodeResult::Failed;
	}

	BlackboardComponent->SetValueAsBool(HasReceivedAttackFromPlayerKey.SelectedKeyName, false);
	return EBTNodeResult::Succeeded;
}
