#include "Components/CIKComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

UCIKComponent::UCIKComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCIKComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (OwnerCharacter == NULL)
		DestroyComponent();

	ScaledHalfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

void UCIKComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float offset = 0.0f;

	float leftDistance;
	FRotator leftRotation;

	float rightDistance;
	FRotator rightRotation;

	Trace(LeftSocket, leftDistance, leftRotation);
	Trace(RightSocket, rightDistance, rightRotation);

	offset = FMath::Min(leftDistance, rightDistance);
	offset = offset > 0.0f ? 0.0f : offset;

	FeetData.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(FeetData.PelvisDistance.Z, offset, DeltaTime, InterpSpeed);

	FeetData.LeftDistance.X = UKismetMathLibrary::FInterpTo(FeetData.LeftDistance.X, leftDistance - offset, DeltaTime, InterpSpeed);
	FeetData.RightDistance.X = UKismetMathLibrary::FInterpTo(FeetData.RightDistance.X, -(rightDistance - offset), DeltaTime, InterpSpeed);

	//FeetData.LeftDistance.X = UKismetMathLibrary::FInterpTo(FeetData.LeftDistance.X, leftDistance, DeltaTime, InterpSpeed);
	//FeetData.RightDistance.X = UKismetMathLibrary::FInterpTo(FeetData.RightDistance.X, -(rightDistance), DeltaTime, InterpSpeed);

	FeetData.LeftRotation = UKismetMathLibrary::RInterpTo(FeetData.LeftRotation, leftRotation, DeltaTime, InterpSpeed);
	FeetData.RightRotation = UKismetMathLibrary::RInterpTo(FeetData.RightRotation, rightRotation, DeltaTime, InterpSpeed);

}

void UCIKComponent::Trace(FName InSocket, float& OutDistance, FRotator& OutRotation)
{
	OutDistance = 0.0f;
	OutRotation = FRotator::ZeroRotator;

	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation(InSocket);
	FVector start = FVector(location.X, location.Y, OwnerCharacter->GetActorLocation().Z);

	float traceZ = start.Z - ScaledHalfHeight - TraceDistance;
	FVector end = FVector(location.X, location.Y, traceZ);

	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);

	FHitResult hit;
	EDrawDebugTrace::Type drawDebugType = EDrawDebugTrace::None;
	if (bDrawDebug)
		drawDebugType = EDrawDebugTrace::ForOneFrame;

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, ignoreActors, drawDebugType, hit, true, FLinearColor::Green, FLinearColor::Red);

	if (hit.IsValidBlockingHit() == false)
		return;

	float length = (hit.ImpactPoint - hit.TraceEnd).Size();

	OutDistance = OffsetDistance + length - TraceDistance;

	FVector normal = hit.ImpactNormal;
	float roll = UKismetMathLibrary::DegAtan2(normal.Y, normal.Z);
	float pitch = -UKismetMathLibrary::DegAtan2(normal.X, normal.Z);

	OutRotation = FRotator(pitch, 0.0f, roll);
}