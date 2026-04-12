#include "SPlayerController.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"

void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(InputToggleMenu, ETriggerEvent::Started, this, &ASPlayerController::TogglePauseMenu);
}

void ASPlayerController::TogglePauseMenu()
{
	
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromViewport();
		PauseMenuInstance = nullptr;
		
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		
		if (APawn* ControlledPawn = GetPawn())
		{
			ControlledPawn->EnableInput(this);
		}
		return;
	}
	
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (!PauseMenuInstance) return;
	
	bShowMouseCursor = true;
	SetInputMode(FInputModeGameAndUI());
	PauseMenuInstance->AddToViewport(100);
	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->DisableInput(this);
	}
}

void ASPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	OnPlayerStateReceived.Broadcast(PlayerState);
}
