#include "VBAbilityComponent.h"

UVBAbilityComponent::UVBAbilityComponent()
{
	InitializeDomainLoadouts();
}

void UVBAbilityComponent::InitializeDomainLoadouts()
{
	for (uint8 i = 0; i < static_cast<uint8>(EAbilityDomain::MAX); ++i)
	{
		EAbilityDomain Domain = static_cast<EAbilityDomain>(i);

		DomainLoadouts.FindOrAdd(Domain);
	}
}

FAbilityLoadout* UVBAbilityComponent::GetActiveDomainLoadout()
{
	return DomainLoadouts.Find(ActiveDomain);
}