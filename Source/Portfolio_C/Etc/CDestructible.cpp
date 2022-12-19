#include "Etc/CDestructible.h"
#include "Global.h"
#include "DestructibleComponent.h"

#pragma warning(disable : 4996)

/*
* UDestructibleComponent Class is deprecated, so later version will have other component.
* Don't use UDestructibleComponent except this class.
*/

ACDestructible::ACDestructible()
{
	PrimaryActorTick.bCanEverTick = true;

	Destructible = CreateDefaultSubobject<UDestructibleComponent>("Destructible");
	RootComponent = Destructible;
}

void ACDestructible::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHp = MaxHp;
	Destructible->SetSimulatePhysics(false);
}

float ACDestructible::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHp > 0.0f)
		CurrentHp -= DamageAmount;

	if (CurrentHp <= 0.0f)
	{
		Destructible->SetSimulatePhysics(true);
		Destructible->ApplyDamage(DamageAmount, DamageCauser->GetActorLocation(),
			GetActorLocation() - DamageCauser->GetActorLocation(), DamageAmount);
		Destructible->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return CurrentHp;
}

