#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"


class USWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOMLOOMANCOURSE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void PrimaryInteract();
	
	USInteractionComponent();

protected:
	
	UPROPERTY()
	TObjectPtr<AActor> FocusedActor;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass;
	
	UPROPERTY()
	TObjectPtr<USWorldUserWidget> DefaultWidgetInstance;
	
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;
	
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius;
	
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;
	
	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);
	
	void FindBestInteractable();
	
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	
};
