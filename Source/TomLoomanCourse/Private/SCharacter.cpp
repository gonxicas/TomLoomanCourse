// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actions/SActionComponent.h"
#include "Actions/SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ASCharacter::ASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");

	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>("InteractionComponent");

	ActionComponent = CreateDefaultSubobject<USActionComponent>("ActionComponent");
	
	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	TimeToHitParamName = "TimeToHit";
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

void ASCharacter::Move(const FInputActionValue& ActionValue)
{
	auto Value = ActionValue.Get<FVector2d>();

	auto ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;

	AddMovementInput(ControlRot.Vector(), Value.Y);

	auto RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, Value.X);
}

void ASCharacter::Look(const FInputActionValue& ActionValue)
{
	const auto Value = ActionValue.Get<FVector2d>();

	AddControllerPitchInput(-Value.Y);
	AddControllerYawInput(Value.X);
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComponent, float NewHealth,
                                  float Delta)
{
	if (Delta < .0f && NewHealth > .0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
	}

	if (NewHealth <= .0f && Delta < .0f)
	{
		auto PlayerController = Cast<APlayerController>(GetController());
		DisableInput(PlayerController);
	}
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

void ASCharacter::PrimaryAttack()
{
	ActionComponent->StartAction(this, "PrimaryAttack");
}

void ASCharacter::SprintStart()
{
	ActionComponent->StartAction(this, "Sprint");
}

void ASCharacter::SprintStop()
{
	ActionComponent->StopAction(this, "Sprint");
}

void ASCharacter::PrimaryAbility()
{
	ActionComponent->StartAction(this, "PrimaryAbility");
}

void ASCharacter::SecondaryAbility()
{
	ActionComponent->StartAction(this, "SecondaryAbility");
}


void ASCharacter::PrimaryInteract()
{
	if (!InteractionComponent) return;

	InteractionComponent->PrimaryInteract();
}

void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	const APlayerController* PlayerController = GetController<APlayerController>();
	const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	Subsystem->AddMappingContext(DefaultInputMapping, 0);

	Input->BindAction(Input_MoveForward, ETriggerEvent::Triggered, this, &ASCharacter::Move);
	Input->BindAction(Input_Turn, ETriggerEvent::Triggered, this, &ASCharacter::Look);

	Input->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryAttack);
	Input->BindAction(Input_PrimaryAbility, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryAbility);
	Input->BindAction(Input_SecondaryAbility, ETriggerEvent::Triggered, this, &ASCharacter::SecondaryAbility);
	Input->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);

	Input->BindAction(Input_PrimaryInteract, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryInteract);

	Input->BindAction(Input_PrimaryStartSprint, ETriggerEvent::Triggered, this, &ASCharacter::SprintStart);
	Input->BindAction(Input_PrimaryStopSprint, ETriggerEvent::Triggered, this, &ASCharacter::SprintStop);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
}

void ASCharacter::HealSelf(float Amount)
{
	AttributeComponent->ApplyHealthChange(this, Amount);
}
