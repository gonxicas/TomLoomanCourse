#include "SCreditSystem.h"

ASCreditSystem::ASCreditSystem() : CurrentCredits(0)
{
}

bool ASCreditSystem::ModifyCredits(int CreditsToAdd)
{
	if (!HasEnoughCredits(-CreditsToAdd))
	{
		return false;
	}

	CurrentCredits += CreditsToAdd;

	OnCreditsChanged.Broadcast(CurrentCredits, CreditsToAdd);

	return true;
}

bool ASCreditSystem::HasEnoughCredits(int CreditsToSpend) const
{
	return CurrentCredits - CreditsToSpend >= 0;
}
