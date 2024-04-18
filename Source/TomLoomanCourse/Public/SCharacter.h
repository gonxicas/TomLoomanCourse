// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

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
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultInputMapping;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_MoveForward;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Turn;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_PrimaryAttack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Jump;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_PrimaryInteract;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& ActionValue);
	void Look(const FInputActionValue& ActionValue);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Test();
	void PrimaryAttack() ;
	void PrimaryInteract();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
