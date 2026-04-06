#include "SPlayerController.h"

void ASPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	OnPlayerStateReceived.Broadcast(PlayerState);
}
