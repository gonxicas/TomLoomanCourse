#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SAnimInstance.generated.h"

class USActionComponent;

UCLASS()
class TOMLOOMANCOURSE_API USAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bIsStunned;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	TObjectPtr<USActionComponent> ActionComponent;

public:
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
