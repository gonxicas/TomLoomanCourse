#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SWorldUserWidget.generated.h"

class USizeBox;

UCLASS()
class TOMLOOMANCOURSE_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	AActor* GetAttachedActor() const {return AttachedActor;}
	void SetAttachedActor(AActor* Actor) {AttachedActor = Actor;}
	
protected:

	UPROPERTY(meta = (BindWidget))
	USizeBox* ParentSizeBox;
	
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	AActor* AttachedActor;

	UPROPERTY(EditAnywhere, Category = "UI")
	FVector WorldOffset;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
