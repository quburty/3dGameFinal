#include "CAction_ShieldSkill.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"

bool UCAction_ShieldSkill::Action()
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

void UCAction_ShieldSkill::Begin_Action()
{
	Super::Begin_Action();

}

void UCAction_ShieldSkill::End_Action()
{
	Super::End_Action();

	State->SetStateIdle();
	State->SetMove();
}

void UCAction_ShieldSkill::HitCharacter(AActor* DamagedActor, AController* EventInstigator, AActor* DamageCauser)
{
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(DamagedActor);

	if (!!state && state->IsStateAction())
	{
		Super::HitCharacter(DamagedActor, EventInstigator, DamageCauser);

	}

}
