#include "Components/CParkourComponent.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CCombatComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCParkourComponent::UCParkourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter == NULL)
	{
		Log_Warning("CParkourComponent can only be attached to character actor")
		DestroyComponent();

	}
	else
	{
		State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
		
	}

}

void UCParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (State->IsStateJump()
		&& OwnerCharacter->GetVelocity().Z < 0.0f
		&& IsCanParkour(OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorForwardVector()))
		State->SetStateParkour();

	if (State->IsStateParkour() == true && bParkour == false)
		Begin_Parkour();

	if (State->IsStateParkour() == false && bParkour == true)
		End_Parkour();

	if (bParkour)
		ParkourTick(DeltaTime);

}

void UCParkourComponent::ParkourLeft(float DeltaTime)
{
	CheckFalse(bParkour);
	CheckFalse(State->IsCanMove());

	FVector moveLocation = OwnerCharacter->GetActorLocation()
		- OwnerCharacter->GetActorRightVector() * ParkourSpeed*DeltaTime;

	FVector offset = -OwnerCharacter->GetActorRightVector() * OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();

	if (IsCanParkour(moveLocation + FVector(0, 0, ZOffset) + offset, OwnerCharacter->GetActorForwardVector()))
	{
		OwnerCharacter->SetActorLocation(moveLocation);
	}
	
}

void UCParkourComponent::ParkourRight(float DeltaTime)
{
	CheckFalse(bParkour);
	CheckFalse(State->IsCanMove());

	FVector moveLocation = OwnerCharacter->GetActorLocation()
		+ OwnerCharacter->GetActorRightVector() * ParkourSpeed*DeltaTime;

	FVector offset = OwnerCharacter->GetActorRightVector() * OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();

	if (IsCanParkour(moveLocation + FVector(0, 0, ZOffset) + offset, OwnerCharacter->GetActorForwardVector()))
	{
		OwnerCharacter->SetActorLocation(moveLocation);
	}
}

void UCParkourComponent::ParkourLeftJump()
{
	CheckFalse(bParkour);
	CheckFalse(State->IsCanMove());

	FVector moveLocation = OwnerCharacter->GetActorLocation()
		- OwnerCharacter->GetActorRightVector() * ParkourJumpCheckWidth;

	if (IsCanParkour(moveLocation + FVector(0, 0, ZOffset), OwnerCharacter->GetActorForwardVector()))
	{
		CheckNull(ParkourJumpLeft.AnimMontage);
		OwnerCharacter->PlayAnimMontage(ParkourJumpLeft.AnimMontage, ParkourJumpLeft.PlayRatio, ParkourJumpLeft.StartSection);

		State->SetStop();
	}
	//else
	//	ParkourCornerLeftJump();
}

void UCParkourComponent::ParkourRightJump()
{
	CheckFalse(bParkour);
	CheckFalse(State->IsCanMove());

	FVector moveLocation = OwnerCharacter->GetActorLocation()
		+ OwnerCharacter->GetActorRightVector() * ParkourJumpCheckWidth;

	if (IsCanParkour(moveLocation + FVector(0, 0, ZOffset), OwnerCharacter->GetActorForwardVector()))
	{
		CheckNull(ParkourJumpRight.AnimMontage);
		OwnerCharacter->PlayAnimMontage(ParkourJumpRight.AnimMontage, ParkourJumpLeft.PlayRatio, ParkourJumpLeft.StartSection);

		State->SetStop();
	}
	//else
	//	ParkourCornerRightJump();
}

void UCParkourComponent::ParkourClimbUp()
{
	CheckFalse(bParkour);
	CheckFalse(State->IsCanMove());
	CheckFalse(IsCanClimbUp());

	CheckNull(ClimbUp.AnimMontage);
	OwnerCharacter->PlayAnimMontage(ClimbUp.AnimMontage, ClimbUp.PlayRatio, ClimbUp.StartSection);

	OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	State->SetStop();

	bClimbUp = true;
}

void UCParkourComponent::EndParkourAction()
{
	State->SetMove();

	OwnerCharacter->GetCharacterMovement()->Velocity = FVector(0);
	OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	if (bClimbUp)
	{
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		State->SetStateIdle();
		bClimbUp = false;
	}
}

void UCParkourComponent::ParkourTick(float DeltaTime)
{
	//CLog::Print("Parkour");

	ParkourVelocity = (OwnerCharacter->GetActorLocation() - PrevCharacterLocation) / DeltaTime;

	ParkourVelocity = ParkourVelocity.RotateAngleAxis(-OwnerCharacter->GetActorRotation().Yaw, OwnerCharacter->GetActorUpVector());
	//CLog::Print(ParkourVelocity);

	PrevCharacterLocation = OwnerCharacter->GetActorLocation();
}

void UCParkourComponent::Begin_Parkour()
{
	bParkour = true;

	OwnerCharacter->GetMesh()->AddRelativeLocation(MeshAdjust);

	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	OwnerCharacter->GetCharacterMovement()->GravityScale = 0.0f;
	OwnerCharacter->GetCharacterMovement()->Velocity = FVector(0);

	OwnerCharacter->SetActorLocation(ParkourLocation);
	OwnerCharacter->SetActorRotation(ParkourRotation);

	PrevCharacterLocation = OwnerCharacter->GetActorLocation();
}

void UCParkourComponent::End_Parkour()
{
	bParkour = false;

	OwnerCharacter->GetMesh()->AddRelativeLocation(-MeshAdjust);

	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	OwnerCharacter->GetCharacterMovement()->GravityScale = 1.0f;
}

bool UCParkourComponent::IsCanParkour(FVector InCharacterLocation, FVector InCharacterForwardVector)
{
	UCCombatComponent* combat = CHelpers::GetComponent<UCCombatComponent>(OwnerCharacter);
	if (!!combat)
	{
		if (combat->GetCurrentCombatIndex() > -1)
			return false;
	}

	FVector wallTop, wallFront;

	float halfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	float radius = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();

	FVector direction = InCharacterForwardVector;

	//FVector start = CharacterLocation + (direction*(radius + 5.0f));
	FVector start = InCharacterLocation;
	FVector end = start + (direction*ParkourCheckDistance);

	FHitResult hitResult;
	bool result;
	
	EDrawDebugTrace::Type drawDebugTrace;
	if (bDrawDebugLine)
		drawDebugTrace = EDrawDebugTrace::ForDuration;
	else
		drawDebugTrace = EDrawDebugTrace::None;

	result = UKismetSystemLibrary::LineTraceSingle(GetWorld(), start + FVector(0,0,HandsZOffset), end + FVector(0, 0, HandsZOffset),
		TraceQuery, false, TArray<AActor*>(), drawDebugTrace, hitResult, true);

	CheckFalseResult(result, false);
	
	wallFront = hitResult.ImpactPoint;
	ParkourRotation = hitResult.ImpactNormal.Rotation();
	ParkourRotation.Yaw += 180.f;

	//FrontNormal = hitResult.ImpactNormal;

	start.Z += halfHeight;
	end.Z += halfHeight;

	result = UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, TraceQuery, false, TArray<AActor*>(),
		drawDebugTrace, hitResult, true);
	CheckTrueResult(result, false);

	start = end;
	end = start;
	end.Z -= ParkourCheckHeight + ZOffset;

	result = UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, TraceQuery, false, TArray<AActor*>(),
		drawDebugTrace, hitResult, true);
	CheckFalseResult(result, false);

	wallTop = hitResult.ImpactPoint;
	//UpNormal = hitResult.ImpactNormal;
	
	//InOutCharacterLocation.Z = wallTop.Z -= halfHeight;
	ParkourLocation = InCharacterLocation;

	ParkourLocation.Z = wallTop.Z - halfHeight;

	return true;
}

bool UCParkourComponent::IsCanClimbUp()
{
	FVector location = OwnerCharacter->GetActorLocation();
	FVector forward = OwnerCharacter->GetActorForwardVector();
	float halfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	float radius = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();

	FVector start = location;
	start.Z += halfHeight + radius + 1.0f;
	start += forward * radius * 2.0f;

	FVector end = start;
	end.Z += halfHeight * 2;

	FHitResult hitResult;
	EDrawDebugTrace::Type drawDebugTrace = EDrawDebugTrace::None;
	if (bDrawDebugLine)
		drawDebugTrace = EDrawDebugTrace::ForDuration;

	bool result = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), start, end, radius, TraceQuery,
		false, TArray<AActor*>(), drawDebugTrace, hitResult, true);

	CheckTrueResult(result, false);

	return true;

}

void UCParkourComponent::ParkourCornerLeftJump()
{
	CheckFalse(bParkour);
	CheckFalse(State->IsCanMove());

	FVector moveLocation = OwnerCharacter->GetActorLocation()
		- OwnerCharacter->GetActorRightVector() * ParkourJumpCheckWidth
		+ OwnerCharacter->GetActorForwardVector() * ParkourJumpCheckWidth;

	if (IsCanParkour(moveLocation + FVector(0, 0, ZOffset), OwnerCharacter->GetActorRightVector()))
	{
		CheckNull(ParkourCornerLeft.AnimMontage);
		OwnerCharacter->PlayAnimMontage(ParkourCornerLeft.AnimMontage, ParkourCornerLeft.PlayRatio, ParkourCornerLeft.StartSection);

		State->SetStop();
	}

}

void UCParkourComponent::ParkourCornerRightJump()
{
	CheckFalse(bParkour);
	CheckFalse(State->IsCanMove());

	FVector moveLocation = OwnerCharacter->GetActorLocation()
		+ OwnerCharacter->GetActorRightVector() * ParkourJumpCheckWidth
		+ OwnerCharacter->GetActorForwardVector() * ParkourJumpCheckWidth;

	if (IsCanParkour(moveLocation + FVector(0, 0, ZOffset), -OwnerCharacter->GetActorRightVector()))
	{
		CheckNull(ParkourCornerRight.AnimMontage);
		OwnerCharacter->PlayAnimMontage(ParkourCornerRight.AnimMontage, ParkourCornerRight.PlayRatio, ParkourCornerRight.StartSection);

		State->SetStop();
	}
}