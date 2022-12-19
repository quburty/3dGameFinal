#include "CAction_MeleeCombo.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Characters/CPlayer.h"

bool UCAction_MeleeCombo::Action()
{
	CheckFalseResult(Super::Action(), false);

	if (bEnable)
	{
		bExist = true;
		bEnable = false;
		return true;
	}
	
	CheckNullResult(State, false);
	
	if (State->IsStateIdle())
		MeleeAttack();
	else if (State->IsStateJump())
		MeleeAirAttack();

	return true;
}

void UCAction_MeleeCombo::MeleeAttack()
{
	CheckFalse(Index < AttackDatas.Num());

	const FAttackData& data = AttackDatas[Index];

	CheckNull(data.MontageData.AnimMontage);
	CheckNull(State);

	State->SetStateAction();
	data.MontageData.bCanMove ? State->SetMove() : State->SetStop();

	Owner->PlayAnimMontage(data.MontageData.AnimMontage, data.MontageData.PlayRatio, data.MontageData.StartSection);
	Power = data.Power;

}

void UCAction_MeleeCombo::MeleeAirAttack()
{
	CheckFalse(Index < AttackDatas.Num());

	const FAttackData& data = AttackDatas[Index];

	CheckNull(data.MontageData.AnimMontage);
	CheckNull(State);

	State->SetStateAirAction();
	Owner->GetCharacterMovement()->Velocity = FVector(0);

	Begin_AirAction();

	data.MontageData.bCanMove ? State->SetMove() : State->SetStop();

	Owner->PlayAnimMontage(data.MontageData.AnimMontage, data.MontageData.PlayRatio, data.MontageData.StartSection);
	Power = data.Power;

}

void UCAction_MeleeCombo::Begin_Action()
{
	Super::Begin_Action();

	if (bExist)
	{
		bExist = false;
		Index++;

		if (State->IsStateAirAction())
			MeleeAirAttack();
		else
			MeleeAttack();
	}

}

void UCAction_MeleeCombo::End_Action()
{
	CheckNull(State);

	if (State->IsStateAirAction())
		End_AirAction();

	bEnable = false;
	bExist = false;
	Index = 0;
}

void UCAction_MeleeCombo::Begin_EnableCombo()
{
	bEnable = true;
}

void UCAction_MeleeCombo::End_EnableCombo()
{
	bEnable = false;
}

void UCAction_MeleeCombo::Begin_AirAction()
{
	Owner->GetCharacterMovement()->GravityScale = 0.0f;

	Owner->bUseControllerRotationPitch = true;
	Owner->bUseControllerRotationYaw = true;

	ACPlayer* player = Cast<ACPlayer>(Owner);
	CheckNull(player);

	player->BeginPostProcess(PostEffect);
}

void UCAction_MeleeCombo::End_AirAction()
{
	Owner->GetCharacterMovement()->GravityScale = 1.0f;

	Owner->bUseControllerRotationPitch = false;
	//Owner->bUseControllerRotationYaw = false;

	FRotator rot = Owner->GetController()->GetControlRotation();
	rot.Pitch = 0.0f;
	rot.Roll = 0.0f;

	//Owner->GetController()->SetControlRotation(rot);
	Owner->SetActorRelativeRotation(rot);

	ACPlayer* player = Cast<ACPlayer>(Owner);
	CheckNull(player);

	player->EndPostProcess();
}

void UCAction_MeleeCombo::HitCharacter(AActor * DamagedActor, AController * EventInstigator, AActor * DamageCauser)
{
	Super::HitCharacter(DamagedActor, EventInstigator, DamageCauser);

}

