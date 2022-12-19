#include "CPoison.h"
#include "Global.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ACPoison::ACPoison()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Effect = CreateDefaultSubobject<UParticleSystemComponent>("Effect");
	RootComponent = Effect;

	//Set Particle reference here.
	CHelpers::GetAsset<UParticleSystem>(&Effect->Template, "ParticleSystem'/Game/Effects/InfinityBladeEffects/P_Poisontest.P_Poisontest'");
}

// Called when the game starts or when spawned
void ACPoison::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner() == NULL)
	{
		Destroy();
	}

	FTimerDynamicDelegate delPoison, delDestroy;

	delPoison.BindDynamic(this, &ACPoison::ApplyPoisonDamage);
	UKismetSystemLibrary::K2_SetTimerDelegate(delPoison, 1.f, true);

	delDestroy.BindDynamic(this, &ACPoison::SelfDestroy);
	UKismetSystemLibrary::K2_SetTimerDelegate(delDestroy, 5.f, false);
}

// Called every frame
void ACPoison::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPoison::ApplyPoisonDamage()
{
	CheckNull(GetOwner());

	GetOwner()->TakeDamage(1.f, FDamageEvent(), NULL, this);
}

void ACPoison::SelfDestroy()
{
	Destroy();
}
