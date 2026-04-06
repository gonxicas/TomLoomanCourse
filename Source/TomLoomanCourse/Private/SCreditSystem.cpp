#include "SCreditSystem.h"

#include "Net/UnrealNetwork.h"

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

void ASCreditSystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCreditSystem, CurrentCredits);
}
