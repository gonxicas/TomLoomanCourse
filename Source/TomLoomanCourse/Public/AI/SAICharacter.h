#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"


class USActionComponent;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USActionComponent> ActionComponent;

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector HasReceivedAttackFromPlayerKey;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PlayerSpotWidgetClass;
	
	UPROPERTY()
	TObjectPtr<USWorldUserWidget> ActiveHealthBar;
	
	UPROPERTY()
	TObjectPtr<USWorldUserWidget> ActivePlayerSpotWidget;


	virtual void PostInitializeComponents() override;
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastDisplayPlayerSpotWidget();

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth, float Delta);

	void SetTargetActor(AActor* NewTarget);
	
	AActor* GetTargetActor() const;
	
};
