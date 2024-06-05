#include "SCreditSystem.h"

ASCreditSystem::ASCreditSystem() : CurrentCredits(0)
{
}

bool ASCreditSystem::ModifyCredits(float CreditsToAdd)
{
	if(CurrentCredits + CreditsToAdd < 0)
	{
		return false;
	}

	CurrentCredits += CreditsToAdd;

	OnCreditsChanged.Broadcast(CurrentCredits, CreditsToAdd);

	return true;
}
