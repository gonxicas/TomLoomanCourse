// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

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
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultInputMapping;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_MoveForward;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Turn;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveForward(const FInputActionValue& ActionValue);
	void Turn(const FInputActionValue& ActionValue);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Test();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
