#include "CStatusComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CharacterInterface.h"

UCStatusComponent::UCStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	bIsDead = false;
}

void UCStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCStatusComponent::ReduceHealth(float InAmount)
{
	if (bIsDead)
		return;

	if (InAmount >= Health)
	{
		bIsDead = true;
		Health = 0.f;
		
		return;

	}
	Health -= InAmount;
	SuperArmorDamagedHealth += InAmount;
}

bool UCStatusComponent::IsSuperArmorDamagedEnough()
{
	if (SuperArmorDamagedHealth > MaxHealth * SuperArmorHpRate)
	{
		SuperArmorDamagedHealth = 0.f;
		return true;
	}
	else
		return false;
}