#include "Etc/CThrow.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"


ACThrow::ACThrow()
{
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	RootComponent = Sphere;

	Particle = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	Particle->AttachToComponent(Sphere, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile");
	Projectile->ProjectileGravityScale = 0.f;
	
}

void ACThrow::SetProjectileVelocity(FVector InVelocity)
{
	Projectile->Velocity = InVelocity;
}

void ACThrow::BeginPlay()
{
	Super::BeginPlay();
	
	LeftTime = LifeTime;
	Sphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACThrow::BeginOverlap);
}

void ACThrow::Tick(float DeltaTime)
{
	LeftTime -= DeltaTime;

	if (LeftTime < 0.0f)
		Destroy();
}

void ACThrow::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AController* controller = NULL;

	if(!!GetOwner())
		controller = GetOwner()->GetInstigatorController();

	if (GetOwner() == OtherActor)
		return;

	CheckNull(Cast<ACharacter>(OtherActor));

	//OtherActor->ReceiveAnyDamage(Damage, NULL, controller, this);
	OtherActor->TakeDamage(Damage, FDamageEvent(), controller, this);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetTransform());

	Destroy();
}