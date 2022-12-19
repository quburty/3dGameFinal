#include "CAction_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"

bool UCAction_Melee::Action()
{
	CheckFalseResult(Super::Action(), false);

	CheckNullResult(State, false);
	CheckFalseResult(State->IsStateIdle(), false);
	CheckFalseResult(AttackDatas.Num() > 0, false);

	const FAttackData& data = AttackDatas[0];
	Owner->PlayAnimMontage(data.MontageData.AnimMontage, data.MontageData.PlayRatio, data.MontageData.StartSection);
	data.MontageData.bCanMove ? State->SetMove() : State->SetStop();

	State->SetStateAction();

	return true;
}

void UCAction_Melee::Begin_Action()
{
	Super::Begin_Action();

}

void UCAction_Melee::End_Action()
{
	Super::End_Action();

}

void UCAction_Melee::HitCharacter(AActor * DamagedActor, AController * EventInstigator, AActor * DamageCauser)
{
	Super::HitCharacter(DamagedActor, EventInstigator, DamageCauser);

}

