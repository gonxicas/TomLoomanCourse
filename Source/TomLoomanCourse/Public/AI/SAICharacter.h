#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"


class USWorldUserWidget;
class USAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class TOMLOOMANCOURSE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASAICharacter();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	USAttributeComponent* AttributeComponent;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector HasReceivedAttackFromPlayerKey;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	
	USWorldUserWidget* ActiveHealthBar;

	virtual void PostInitializeComponents() override;
	
	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth, float Delta);

	void SetTargetActor(AActor* NewTarget);
	
};
