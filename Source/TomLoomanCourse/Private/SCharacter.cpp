// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::MoveForward(const FInputActionValue& ActionValue)
{
	float value = ActionValue.Get<float>();

	AddMovementInput(GetActorForwardVector(), value);
}

void ASCharacter::Turn(const FInputActionValue& ActionValue)
{
	const auto Value = ActionValue.Get<float>();

	AddControllerYawInput(Value);
}

// Called every frame
void ASCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASCharacter::Test()
{
	AddMovementInput(GetActorForwardVector(), 1);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	const APlayerController* PlayerController = GetController<APlayerController>();
	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	Subsystem->AddMappingContext(DefaultInputMapping, 0);
	
    Input->BindAction(Input_MoveForward, ETriggerEvent::Triggered, this, &ASCharacter::MoveForward);
	Input->BindAction(Input_Turn, ETriggerEvent::Triggered, this, &ASCharacter::Turn);

}

