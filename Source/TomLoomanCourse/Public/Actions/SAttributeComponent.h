#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnAttributeChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewValue, float, Delta);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOMLOOMANCOURSE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USAttributeComponent();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
	static bool IsActorAlive(AActor* Actor);
		
	virtual void BeginPlay() override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float MaxHealth;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float Rage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Attribute")
	float MaxRage;
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRageChanged(AActor* InstigatorActor, float NewRage, float Delta);

public:
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnRageChanged;

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const {return Health > 0.f;}

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void ApplyRageChange(float RageAmount);
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool TryUseRage(float RageCost);
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool CanAffordRage(float RageCost);
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool HasMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool HealToMaxHealth(AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetCurrentHealthPercentage() const {return Health / MaxHealth;}

	bool Kill(AActor* InstigatorActor);

};
