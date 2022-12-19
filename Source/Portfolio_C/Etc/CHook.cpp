#include "Etc/CHook.h"
#include "Global.h"
#include "Etc/IHookable.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACHook::ACHook()
{
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	RootComponent = Sphere;

	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile");
	Projectile->ProjectileGravityScale = 0.f;

	Spline = CreateDefaultSubobject<USplineMeshComponent>("Spline");
	Spline->SetMobility(EComponentMobility::Movable);
	Spline->AttachToComponent(Sphere, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
}

void ACHook::SetProjectileDirection(FVector InDirection)
{
	SetActorRotation(InDirection.Rotation());
	InDirection.Normalize();
	Projectile->Velocity = Projectile->InitialSpeed*InDirection;
}

void ACHook::BeginPlay()
{
	Super::BeginPlay();

	bHitted = false;
	LeftTime = LifeTime;
	Sphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACHook::BeginOverlap);

	CheckNull(GetOwner());

	Movement = CHelpers::GetComponent<UCharacterMovementComponent>(GetOwner());
	State = CHelpers::GetComponent<UCStateComponent>(GetOwner());
}

void ACHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LeftTime -= DeltaTime;

	if (LeftTime < 0.0f)
	{
		Destroy();
		return;
	}

	CheckNull(Movement);
	CheckNull(State);
	CheckNull(GetOwner());

	//사용자 이동시키는 코드
	if (bHitted && State->IsStateHook())
	{
		if (FVector::Distance(this->GetActorLocation(), GetOwner()->GetActorLocation())<Sphere->GetScaledSphereRadius())
		{
			Destroy();
			return;
		}

		FVector force = this->GetActorLocation() - GetOwner()->GetActorLocation();

		force.Normalize();
		force *= FlySpeed;

		Movement->Velocity = force;
		Movement->GravityScale = 0.0f;
		Movement->SetMovementMode(MOVE_Falling);

		FRotator rot = GetOwner()->GetActorRotation();
		rot.Yaw = force.Rotation().Yaw;
		GetOwner()->SetActorRotation(rot);
	}

	CheckFalse(bUseSpline);

	FTransform transform = Spline->GetComponentTransform();
	FQuat tangent = transform.InverseTransformRotation(GetActorRotation().Quaternion());
	//FVector startPos = transform.InverseTransformPosition(GetOwner()->GetActorLocation());
	FVector startPos = transform.InverseTransformPosition
	(
		CHelpers::GetComponent<USkeletalMeshComponent>(GetOwner())->GetSocketLocation(HandSocket)
	);
	
	Spline->SetStartAndEnd(startPos, tangent.GetForwardVector(), FVector(0), tangent.GetForwardVector());
}

void ACHook::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (!!Movement && bHitted)
	{
		Movement->Velocity = FVector(0);
		Movement->GravityScale = 1.0f;
	}

	if (!!State && State->IsStateHook())
		State->SetStateIdle();

	Super::EndPlay(EndPlayReason);
}

void ACHook::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	CheckTrue(bHitted);
	CheckTrue(GetOwner() == OtherActor);
	CheckNull(GetOwner());

	CheckFalse(OtherActor->GetClass()->ImplementsInterface(IIHookable::UClassType::StaticClass()));

	//충돌:overlap generate event 체크되어있어야됨
	if(!!HitEffect)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetTransform());

	bHitted = true;

	//갈고리 발사 정지
	Projectile->Velocity = FVector(0);

	CheckNull(State);
	State->SetStateHook();
}

