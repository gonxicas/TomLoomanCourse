// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USAttributeComponent;
class USInteractionComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class TOMLOOMANCOURSE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	USInteractionComponent* InteractionComponent;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> PrimaryAttackProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> PrimaryAbilityProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastPrimaryAbilityParticleEffect;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> SecondaryAbilityProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim;

	FTimerHandle TimerHandlePrimaryAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_MoveForward;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Turn;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_PrimaryAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_PrimaryAbility;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_SecondaryAbility;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Jump;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_PrimaryInteract;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	void Move(const FInputActionValue& ActionValue);
	void Look(const FInputActionValue& ActionValue);
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth, float Delta);


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Test();
	void PrimaryAttack();
	void AdjustSpawnRotationWithTarget(const FVector& HandLocation,
	                                   UE::Math::TRotator<double>& ProjectileRotation) const;
	void PrimaryAttackTimeElapsed();
	void PrimaryAbilityTimeElapsed();
	void SecondaryAbility();
	void SecondaryAbilityTimeElapsed();
	void PrimaryInteract();
	void PrimaryAbility();
	void SpawnProjectile(UClass* Object, const FVector& From);
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
