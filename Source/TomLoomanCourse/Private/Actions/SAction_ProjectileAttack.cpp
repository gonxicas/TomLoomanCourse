#include "Actions/SAction_ProjectileAttack.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

static TAutoConsoleVariable<bool> CVarDebugDrawAttack(TEXT("su.DrawDebugAttack"), false,
															TEXT("Enable debug lines for Character Attack."),
															ECVF_Cheat);

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = .2f;
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (!Character) return;
	
	Character->PlayAnimMontage(AttackAnim);

	UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName,
	                                       FVector::Zero(), FRotator::ZeroRotator, EAttachLocation::SnapToTarget);
	FTimerHandle TimerHandle_AttackDelay;
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);

}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (!ensureAlways(ProjectileCLass)) return;
	
	UE::Math::TRotator<double> ProjectileRotation;
	auto HandLocation = InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);
	AdjustSpawnRotationWithTarget(InstigatorCharacter, HandLocation, ProjectileRotation);

	const auto SpawnTransformMatrix = FTransform(ProjectileRotation, HandLocation);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = InstigatorCharacter;

	GetWorld()->SpawnActor<AActor>(ProjectileCLass, SpawnTransformMatrix, SpawnParameters);
	
	StopAction(InstigatorCharacter);
}

void USAction_ProjectileAttack::AdjustSpawnRotationWithTarget(ACharacter* InstigatorCharacter,
                                                              const FVector& HandLocation, UE::Math::TRotator<double>& ProjectileRotation) const
{
	const auto CameraLocation = InstigatorCharacter->GetPawnViewLocation();
	const auto CameraRotation = InstigatorCharacter->GetControlRotation();
	bool bDebugDraw = CVarDebugDrawAttack.GetValueOnGameThread();


	FHitResult Hit;
	auto End = CameraLocation + CameraRotation.Vector() * 10000;

	FCollisionObjectQueryParams QueryParams;
	FCollisionQueryParams Test;
	Test.AddIgnoredActor(InstigatorCharacter);
	QueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	QueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	QueryParams.AddObjectTypesToQuery(ECC_Pawn);


	auto BlockHit = GetWorld()->LineTraceSingleByObjectType(Hit, CameraLocation, End, QueryParams, Test);

	auto TargetPoint = BlockHit ? Hit.ImpactPoint : End;
	ProjectileRotation = FRotationMatrix::MakeFromX(TargetPoint - HandLocation).Rotator();

	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), CameraLocation, TargetPoint, FColor::Red,
					  false, 2.f, 0, 2.f);
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 30, 32, FColor::Red, false, 2.f);
		DrawDebugLine(GetWorld(), HandLocation, TargetPoint, FColor::Green,
					  false, 2.f, 0, 2.f);
	}
}