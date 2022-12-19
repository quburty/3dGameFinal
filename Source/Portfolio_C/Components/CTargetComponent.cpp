#include "Components/CTargetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"

UCTargetComponent::UCTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCTargetComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UCTargetComponent::ToggleTarget()
{
	if (!!Target)
	{
		EndTargeting();

		return;
	}

	StartTargeting();
}

void UCTargetComponent::ChangeTarget(bool InRight)
{
	CheckNull(Target);

	TMap<float, ACharacter*> map;
	for (ACharacter* character : TraceTargets)
	{
		if (Target == character)
			continue;

		FVector targetLocation = character->GetActorLocation();
		FVector ownerLocation = OwnerCharacter->GetActorLocation();
		FVector ownerToTarget = targetLocation - ownerLocation;

		FQuat quat = FQuat(OwnerCharacter->GetControlRotation());
		FVector forward = quat.GetForwardVector();
		FVector up = quat.GetUpVector();

		FVector cross = forward ^ ownerToTarget;
		float dot = cross | up;

		map.Add(dot, character);
	}


	float minimum = FLT_MAX;
	ACharacter* target = NULL;

	TArray<float> keys;
	map.GetKeys(keys);
	for (float key : keys)
	{
		if (InRight == true)
		{
			if (key < 0.0f)
				continue;
		}
		else
		{
			if (key > 0.0f)
				continue;
		}

		if (FMath::Abs(key) > minimum)
			continue;

		target = *map.Find(key);
		minimum = FMath::Abs(key);
	}

	ChangeCursor(target);
}

void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckNull(Target);

	if (IsValid(Target) == false)
	{
		EndTargeting();

		return;
	}

	if (Target->GetDistanceTo(OwnerCharacter) >= TraceRadius)
	{
		EndTargeting();

		return;
	}

	FVector start = OwnerCharacter->GetActorLocation();
	FVector target = Target->GetActorLocation();

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(start, target);

	FRotator current = OwnerCharacter->GetControlRotation();

	rotator = UKismetMathLibrary::RInterpTo(current, rotator, DeltaTime, InteropSpeed);
	OwnerCharacter->GetController()->SetControlRotation(rotator);
}

void UCTargetComponent::StartTargeting()
{
	SetTraceTargets();
	SetTarget();
}

void UCTargetComponent::EndTargeting()
{
	Target = NULL;
	TraceTargets.Empty();

	if (!!Attached)
		Attached->DestroyComponent();
}

void UCTargetComponent::SetTraceTargets()
{
	CheckNull(OwnerCharacter);

	EDrawDebugTrace::Type drawDebugTrace = EDrawDebugTrace::None;
	if (bDrawDebug)
		drawDebugTrace = EDrawDebugTrace::ForDuration;

	FVector start = OwnerCharacter->GetActorLocation();
	FVector end = start + FVector(0, 0, 1);

	TArray<AActor*> ignoreActors{ OwnerCharacter };

	TArray<FHitResult> hitResults;

	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), start, end, TraceRadius,
		"Pawn", false, ignoreActors, drawDebugTrace, hitResults, true);

	for (const FHitResult& result : hitResults)
	{
		ACharacter* character = Cast<ACharacter>(result.GetActor());

		if (!!character)
			TraceTargets.AddUnique(character);
	}
}

void UCTargetComponent::SetTarget()
{
	CheckNull(OwnerCharacter);

	ACharacter* target = NULL;

	float max = -2.0f;
	for (ACharacter* character : TraceTargets)
	{
		FVector direction = OwnerCharacter->GetActorForwardVector();
		FVector offset = character->GetActorLocation() - OwnerCharacter->GetActorLocation();
		offset.Normalize();

		float dot = FVector::DotProduct(direction, offset);

		if (max < dot)
		{
			max = dot;
			target = character;
		}
	}

	if (!!target)
		ChangeCursor(target);
}

void UCTargetComponent::ChangeCursor(ACharacter* InCharacter)
{
	if (!!InCharacter)
	{
		if (!!Attached)
			Attached->DestroyComponent();

		Attached = UGameplayStatics::SpawnEmitterAttached(Particle, InCharacter->GetMesh(), NAME_None,
			AttachOffset + FVector(0, 0, InCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()*2));
		Target = InCharacter;

		return;
	}

	EndTargeting();
}
