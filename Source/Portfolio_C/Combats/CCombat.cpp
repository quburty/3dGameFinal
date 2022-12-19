#include "CCombat.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Combats/Weapons/CWeapon.h"
#include "Combats/Actions/CAction.h"
#include "Combats/Actions/CAction_MeleeCombo.h"
#include "Characters/CPlayer.h"

UCCombat::UCCombat()
{
}


void UCCombat::Equip()
{
	CheckNull(GetOuter());
	
	ACPlayer* player = Cast<ACPlayer>(GetOuter());
	if (!!player && bLockControlRotate)
		player->SetRotationLock();

	if (!!EquipMontageData.AnimMontage)
	{
		const FAssetMontageData& data = EquipMontageData;
		Cast<ACharacter>(GetOuter())->PlayAnimMontage
		(
			data.AnimMontage,data.PlayRatio,data.StartSection
		);

		if (!!State)
		{
			EquipMontageData.bCanMove ? State->SetMove() : State->SetStop();
			State->SetStateAction();
		}
	}
	else
	{
		GrabWeapon();
		EndEquip();
	}
}

void UCCombat::Sheath()
{
	CheckNull(GetOuter());

	ACPlayer* player = Cast<ACPlayer>(GetOuter());
	if (!!player && bLockControlRotate)
		player->SetRotationFree();

	if (!!SheathMontageData.AnimMontage)
	{
		const FAssetMontageData& data = SheathMontageData;
		Cast<ACharacter>(GetOuter())->PlayAnimMontage
		(
			data.AnimMontage, data.PlayRatio, data.StartSection
		);

		if (!!State)
		{
			SheathMontageData.bCanMove ? State->SetMove() : State->SetStop();
			State->SetStateAction();
		}
	}
	else
	{
		UngrabWeapon();
		EndSheath();
	}

}

UCAction* UCCombat::GetSkillAction(int32 InIndex)
{
	CheckFalseResult(InIndex < SkillActions.Num(), NULL);
	CheckTrueResult(InIndex < 0, NULL);

	return SkillActions[InIndex];
}

void UCCombat::GrabWeapon()
{
	CheckNull(Weapon);

	Weapon->GrabWeapon();
}

void UCCombat::UngrabWeapon()
{
	CheckNull(Weapon);

	Weapon->UngrabWeapon();
}

void UCCombat::EndEquip()
{
	CheckNull(State);
	State->SetMove();
}

void UCCombat::EndSheath()
{
	CheckNull(State);
	State->SetMove();
}

void UCCombat::OnEnableCombo()
{
	CheckNull(AttackAction);

	UCAction_MeleeCombo* melee = Cast<UCAction_MeleeCombo>(AttackAction);
	CheckNull(melee);

	melee->Begin_EnableCombo();
}

void UCCombat::OffEnableCombo()
{
	CheckNull(AttackAction);

	UCAction_MeleeCombo* melee = Cast<UCAction_MeleeCombo>(AttackAction);
	CheckNull(melee);

	melee->End_EnableCombo();
}

void UCCombat::OnCollision()
{
	CheckNull(Weapon);
	Weapon->OnCollision();
}

void UCCombat::OffCollision()
{
	CheckNull(Weapon);
	Weapon->OffCollision();
}

bool UCCombat::IsComboEnable()
{
	CheckNullResult(AttackAction, false);

	UCAction_MeleeCombo* melee = Cast<UCAction_MeleeCombo>(AttackAction);
	CheckNullResult(melee, false);

	return melee->IsComboEnable();
}
