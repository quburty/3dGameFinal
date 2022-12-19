#include "CAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CCombatComponent.h"
//#include "Components/CStateComponent.h"
#include "Components/CParkourComponent.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Character = Cast<ACharacter>(GetOwningActor());

	ParkourVelocity = FVector(0);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);

	CheckNull(Character);

	FVector directionVector = FVector(0, 0, 0);
	Character->GetVelocity().ToDirectionAndLength(directionVector, Speed);

	Direction = CalculateDirection(directionVector, Character->GetActorForwardVector().Rotation());

	bOnAir = Character->GetCharacterMovement()->IsFalling();

	UCCombatComponent* combat = Cast<UCCombatComponent>(Character->GetComponentByClass(UCCombatComponent::StaticClass()));
	CheckNull(combat);

	bIsCombatMode = combat->IsEquipped();

	CombatModeIndex = combat->GetCurrentCombatIndex();

	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(Character);
	CheckNull(state);

	StateType = state->GetStateType();

	bIsParkour = state->IsStateParkour();
	
	if (bIsParkour)
	{
		UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(Character);
		CheckNull(parkour);
		ParkourVelocity = parkour->GetParkourVelocity();
	}

	UCIKComponent* ik = CHelpers::GetComponent<UCIKComponent>(Character);
	if (!!ik)
	{
		IKFeetData = ik->GetFeetData();
	}

}