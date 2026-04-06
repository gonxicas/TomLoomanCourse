#include "SCreditSystem.h"

#include "Net/UnrealNetwork.h"
#include "SaveSystem/SSaveGame.h"

ASCreditSystem::ASCreditSystem() : CurrentCredits(0)
{
}

void ASCreditSystem::Server_ModifyCredits_Implementation(int CreditsToAdd)
{
	if (!HasEnoughCredits(-CreditsToAdd))
	{
		return;
	}

	CurrentCredits += CreditsToAdd;
	OnRep_ModifyCredits(CreditsToAdd);
}

void ASCreditSystem::OnRep_ModifyCredits(int CreditsToAdd) const
{
	OnCreditsChanged.Broadcast(CurrentCredits, CreditsToAdd);
}

bool ASCreditSystem::HasEnoughCredits(int CreditsToSpend) const
{
	return CurrentCredits - CreditsToSpend >= 0;
}

void ASCreditSystem::SavePlayerState_Implementation(USSaveGame* SavedObject)
{
	if (!SavedObject) return;
	
	SavedObject->Credits = CurrentCredits;
	
}

void ASCreditSystem::LoadPlayerState_Implementation(USSaveGame* SavedObject)
{
	if (!SavedObject) return;

	CurrentCredits = SavedObject->Credits;
}

void ASCreditSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCreditSystem, CurrentCredits);
}
