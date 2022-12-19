#include "Combats/Weapons/CFire.h"
#include "Global.h"
#include "Components/SphereComponent.h"

ACFire::ACFire()
{
 	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	RootComponent = Sphere;
}

UShapeComponent* ACFire::GetShapeComponent()
{
	return Sphere;
}

void ACFire::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

