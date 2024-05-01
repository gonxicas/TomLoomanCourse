// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckLowHealth.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if(!ensure(BlackboardComponent))
	{
		return;
	}
	
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (!ensure(MyController))
	{
		return;
	}

	APawn* MyPawn =MyController->GetPawn();

	if (!ensure(MyPawn))
	{
		return;
	}

	USAttributeComponent* MyAttributeComponent = Cast<USAttributeComponent>(
		MyPawn->GetComponentByClass(USAttributeComponent::StaticClass()));

	if(!ensureMsgf(MyAttributeComponent, TEXT("Trying to execute SBT_CheckLowHealth but the AttributeComponent was not found.")))
	{
		return;
	}
	const bool bIsLowHealth = MyAttributeComponent->GetCurrentHealthPercentage() <= LowHealthPercentage;
	BlackboardComponent->SetValueAsBool(IsLowHealthKey.SelectedKeyName, bIsLowHealth);
}
