#include "CAction_SwordSkill.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

bool UCAction_SwordSkill::Action()
{
	CheckFalseResult(Super::Action(), false);

	CheckNullResult(State,false);
	CheckFalseResult(State->IsStateIdle(),false);
	CheckFalseResult(AttackDatas.Num() > 0,false);

	const FAttackData& data = AttackDatas[0];
	Owner->PlayAnimMontage(data.MontageData.AnimMontage, data.MontageData.PlayRatio, data.MontageData.StartSection);
	data.MontageData.bCanMove ? State->SetMove() : State->SetStop();

	State->SetStateAction();

	return true;
}

void UCAction_SwordSkill::Begin_Action()
{
	Super::Begin_Action();

	Owner->LaunchCharacter(Owner->GetActorForwardVector()*5000.f, false, false);	
}

void UCAction_SwordSkill::End_Action()
{
	Super::End_Action();

	Owner->GetCharacterMovement()->Velocity = FVector(0);
	State->SetStateIdle();
	State->SetMove();
}
