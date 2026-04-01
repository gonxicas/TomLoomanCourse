#pragma once

#include "CoreMinimal.h"
#include "SPickUpTemplate.h"
#include "GameFramework/Actor.h"
#include "SPowerUp.generated.h"

class USAction;

UCLASS()
class TOMLOOMANCOURSE_API ASPowerUp : public ASPickUpTemplate
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "PowerUp")
	TSubclassOf<USAction> ActionToGrant;	
	
	virtual bool InteractPrecondition(APawn* InstigatorPawn)const override;
	virtual void InteractAction(APawn* InstigatorPawn) override;
	virtual void PreventInteraction() override;
	virtual void AllowInteraction() override;
	virtual void ResetPickUpDelayed() override;
};
