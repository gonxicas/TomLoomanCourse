// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_Heal.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (!ensure(MyController))
	{
		return EBTNodeResult::Failed;
	}

	ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());

	if (!MyPawn)
	{
		return EBTNodeResult::Failed;
	}

	USAttributeComponent* MyAttributeComponent = USAttributeComponent::GetAttributes(MyPawn);

	if(!ensureMsgf(MyAttributeComponent, TEXT("Trying to execute SBT_TaskHealth but the AttributeComponent was not found.")))
	{
		return EBTNodeResult::Failed;
	}

	MyAttributeComponent->HealToMaxHealth(MyPawn);
	return EBTNodeResult::Succeeded;
}
