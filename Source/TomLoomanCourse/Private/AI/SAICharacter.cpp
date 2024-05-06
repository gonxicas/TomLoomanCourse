#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"


ASAICharacter::ASAICharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");
	TimeToHitParamName = "TimeToHit";

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	DrawDebugString(GetWorld(), GetActorLocation(), TEXT("Player Spotted"),
	                nullptr, FColor::White, 4.0f, true);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth,
                                    float Delta)
{
	if (Delta >= .0f)
	{
		return;
	}

	if(!ActiveHealthBar)
	{
		ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
	}

	if(ActiveHealthBar)
	{
		ActiveHealthBar->SetAttachedActor(this);
		ActiveHealthBar->AddToViewport();
	}

	GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
	if(InstigatorActor != this)
	{
		AAIController* MyAIController = Cast<AAIController>(GetController());
		MyAIController->GetBlackboardComponent()->SetValueAsBool(TEXT("HasReceivedAttackFromPlayer"), true);
		SetTargetActor(InstigatorActor);
	}

	if (NewHealth > .0f)
	{
		return;
	}

	AAIController* AIController = Cast<AAIController>(GetController());
	if(AIController)
	{
		AIController->GetBrainComponent()->StopLogic("Killed");
	}

	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
	
	SetLifeSpan(10.f);
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIController = Cast<AAIController>(GetController());

	if (!AIController)
	{
		return;
	}

	AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	

}