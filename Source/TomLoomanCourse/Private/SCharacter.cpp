// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>("InteractionComponent");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
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
	PlayAnimMontage(AttackAnim);

	GetWorld()->GetTimerManager().SetTimer(TimerHandlePrimaryAttack, this,
	                                       &ASCharacter::PrimaryAttackTimeElapsed, .2f);
}

void ASCharacter::PrimaryAttackTimeElapsed()
{
	const FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

	

	const auto CameraLocation = CameraComponent->GetComponentLocation();
	const auto CameraRotation = CameraComponent->GetComponentRotation();


	FHitResult Hit;
	auto End = CameraLocation + CameraRotation.Vector() * 10000;
	
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);


	auto BlockHit = GetWorld()->LineTraceSingleByObjectType(Hit, CameraLocation, End, QueryParams);
	auto TargetPoint = BlockHit ? Hit.ImpactPoint : End;
	auto ProjectileRotation = FRotationMatrix::MakeFromX(TargetPoint - HandLocation).Rotator();
	
	const auto SpawnTransformMatrix = FTransform(ProjectileRotation, HandLocation);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTransformMatrix, SpawnParameters);
	
	DrawDebugLine(GetWorld(), CameraLocation, TargetPoint, FColor::Red,
	              false, 2.f, 0, 2.f);
	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 30, 32, FColor::Red, false, 2.f);
	DrawDebugLine(GetWorld(), SpawnTransformMatrix.GetLocation(), TargetPoint, FColor::Green,
				  false, 2.f, 0, 2.f);
}

void ASCharacter::PrimaryInteract()
{
	if (!InteractionComponent) return;

	InteractionComponent->PrimaryInteract();
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

	Input->BindAction(Input_MoveForward, ETriggerEvent::Triggered, this, &ASCharacter::Move);
	Input->BindAction(Input_Turn, ETriggerEvent::Triggered, this, &ASCharacter::Look);

	Input->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryAttack);
	Input->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ACharacter::Jump);

	Input->BindAction(Input_PrimaryInteract, ETriggerEvent::Triggered, this, &ASCharacter::PrimaryInteract);
}
