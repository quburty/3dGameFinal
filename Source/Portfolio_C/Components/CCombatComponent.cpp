#include "CCombatComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "DataAssets/DataAsset_Combat.h"
#include "Components/CStateComponent.h"
#include "Combats/CCombat.h"
#include "Combats/Weapons/CWeapon.h"
#include "Combats/Actions/CAction.h"
#include "Combats/Actions/CAction_MeleeCombo.h"
#include "Characters/CharacterInterface.h"
#include "Materials/MaterialInstance.h"

UCCombatComponent::UCCombatComponent()
	: Super()
{
	StartCombat = -1;

	//PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UCCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
	State = Cast<UCStateComponent>(Owner->GetComponentByClass(UCStateComponent::StaticClass()));

	ClearDelegates();

	for (UDataAsset_Combat* dataAsset : DataAssets)
	{
		if (dataAsset->GetWeaponClass() == NULL)
			continue;

		ACWeapon* weapon;

		//Spawn Weapon
		{
			weapon = GetWorld()->SpawnActorDeferred<ACWeapon>(dataAsset->GetWeaponClass(), FTransform(), Owner);
			weapon->FinishSpawning(FTransform());
			weapon->UngrabWeapon();
			weapon->OnWeaponHitDelegate.AddUniqueDynamic(this, &UCCombatComponent::OnHitCharacter);
		}

		UCCombat* combat;

		//Make UCCombat Class
		{
			//combat = GetWorld()->SpawnActor<UCCombat>(FActorSpawnParameters());
			//combat->AttachToActor(Owner, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
			combat = NewObject<UCCombat>(Owner);

			if (!!State)
				combat->SetState(State);

			combat->SetEquipData(dataAsset->GetEquipMontageData());
			combat->SetSheathData(dataAsset->GetSheathMontageData());
			combat->SetLockControlRotate(dataAsset->GetLockRotation());
			combat->SetWeapon(weapon);

			Combats.Add(combat);
		}

		//Make UCAction Class
		{
			//if (dataAsset->GetActionClass() == NULL)
			//	continue;

			//UCAction_MeleeCombo* melee = GetWorld()->SpawnActorDeferred<UCAction_MeleeCombo>
			//	(UCAction_MeleeCombo::StaticClass(), FTransform(), Owner);
			//melee->SetAttackDatas(dataAsset->GetAttackDatas());
			//
			//melee->FinishSpawning(FTransform());

			UCAction_MeleeCombo* melee = NewObject<UCAction_MeleeCombo>(Owner);
			melee->SetAttackDatas(dataAsset->GetAttackDatas());

			combat->SetAttackAction(melee);

			OnTickDelegate.AddUniqueDynamic(melee, &UCAction::Tick);
		}

		//Make Skill Action Class.
		for (const FAttackData_Skill& skill : dataAsset->GetSkillDatas())
		{
			if (skill.SkillActionClass == NULL)
				continue;

			//UCAction* action = GetWorld()->SpawnActorDeferred<UCAction>(skill.SkillActionClass, FTransform(), Owner);
			UCAction* action = NewObject<UCAction>(Owner, skill.SkillActionClass);

			if (action == NULL)
				continue;

			action->AddAttackData(skill);
			action->SetUseCooldown(skill.bUseCooldown);

			combat->AddSkillAction(action);

			OnTickDelegate.AddUniqueDynamic(action, &UCAction::Tick);
		}

	}

	//Setting Start Combat
	{
		CheckFalse(StartCombat >= 0);
		CheckFalse(Combats.Num() > StartCombat);
		CheckNull(Combats[StartCombat]);

		CurrentCombatIndex = StartCombat;
		Combats[StartCombat]->GrabWeapon();
		Combats[StartCombat]->EndEquip();
	}
}

void UCCombatComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//CheckNull(CurrentAction);
	//CLog::Print(CurrentAction->GetName());
}

void UCCombatComponent::Attack()
{
	CheckTrue(CurrentCombatIndex == -1);
	CheckNull(Combats[CurrentCombatIndex]);

	UCAction* action = Combats[CurrentCombatIndex]->GetAttackAction();
	CheckNull(action);

	action->Action();

	CurrentAction = action;

}

void UCCombatComponent::Skill(int32 InSkillIndex)
{
	CheckTrue(CurrentCombatIndex == -1);

	UCAction* action = Combats[CurrentCombatIndex]->GetSkillAction(InSkillIndex);
	CheckNull(action);
	CheckFalse(State->IsStateIdle());
	
	action->Action();

	CurrentAction = action;

}

void UCCombatComponent::OnWeapon(int32 InCombatIndex)
{
	CheckNull(State);
	CheckFalse(InCombatIndex >= 0);
	CheckFalse(Combats.Num() > InCombatIndex);
	CheckNull(Combats[InCombatIndex]);

	if (Combats.Num() > CurrentCombatIndex && CurrentCombatIndex >= 0
		&& Combats[CurrentCombatIndex]->IsComboEnable()
		&& CurrentCombatIndex != InCombatIndex)
	{
		OnSwitchingDelegate.AddUFunction(this, "SwitchingMelee", CurrentCombatIndex, InCombatIndex);

		return;
	}

	CheckFalse(State->IsStateIdle());

	if (InCombatIndex == CurrentCombatIndex)
	{
		Sheath();
		return;
	}

	CheckFalse(CurrentCombatIndex == -1);

	OnGrabWeaponDelegate.AddUniqueDynamic(Combats[InCombatIndex], &UCCombat::GrabWeapon);
	OnEndEquipDelegate.AddUniqueDynamic(Combats[InCombatIndex], &UCCombat::EndEquip);

	CurrentCombatIndex = InCombatIndex;
	Combats[InCombatIndex]->Equip();

}

void UCCombatComponent::Sheath()
{
	CheckNull(State);
	CheckFalse(State->IsStateIdle());
	CheckFalse(Combats.Num() > CurrentCombatIndex);
	CheckNull(Combats[CurrentCombatIndex]);
	CheckTrue(CurrentCombatIndex == -1);
	
	OnUngrabWeaponDelegate.AddUniqueDynamic(Combats[CurrentCombatIndex], &UCCombat::UngrabWeapon);
	OnEndSheathDelegate.AddUniqueDynamic(Combats[CurrentCombatIndex], &UCCombat::EndSheath);

	Combats[CurrentCombatIndex]->Sheath();
	CurrentCombatIndex = -1;

}

void UCCombatComponent::GrabWeapon()
{
	if (OnGrabWeaponDelegate.IsBound())
		OnGrabWeaponDelegate.Broadcast();

	OnGrabWeaponDelegate.Clear();
}

void UCCombatComponent::UngrabWeapon()
{
	if (OnUngrabWeaponDelegate.IsBound())
		OnUngrabWeaponDelegate.Broadcast();

	OnUngrabWeaponDelegate.Clear();
}

void UCCombatComponent::EndEquip()
{
	if (OnEndEquipDelegate.IsBound())
		OnEndEquipDelegate.Broadcast();

	OnEndEquipDelegate.Clear();
}

void UCCombatComponent::EndSheath()
{
	if (OnEndSheathDelegate.IsBound())
		OnEndSheathDelegate.Broadcast();

	OnEndSheathDelegate.Clear();
}

bool UCCombatComponent::IsEquipped()
{
	return (CurrentCombatIndex == -1) ? false : true;
}

void UCCombatComponent::OnEnableCombo()
{
	CheckTrue(CurrentCombatIndex == -1);
	CheckFalse(Combats.Num() > CurrentCombatIndex);
	CheckNull(Combats[CurrentCombatIndex]);

	Combats[CurrentCombatIndex]->OnEnableCombo();
}

void UCCombatComponent::OffEnableCombo()
{
	CheckTrue(CurrentCombatIndex == -1);
	CheckFalse(Combats.Num() > CurrentCombatIndex);
	CheckNull(Combats[CurrentCombatIndex]);

	Combats[CurrentCombatIndex]->OffEnableCombo();
}

void UCCombatComponent::BeginAction()
{
	CheckFalse(Combats.Num() > CurrentCombatIndex);
	CheckNull(Combats[CurrentCombatIndex]);
	CheckTrue(CurrentCombatIndex == -1);

	if (OnSwitchingDelegate.IsBound())
	{
		OnSwitchingDelegate.Broadcast();
		OnSwitchingDelegate.Clear();
		return;
	}

	CheckNull(CurrentAction);
	CurrentAction->Begin_Action();
}

void UCCombatComponent::EndAction()
{
	if (OnSwitchingDelegate.IsBound())
		OnSwitchingDelegate.Clear();

	CheckNull(CurrentAction);
	CurrentAction->End_Action();
	CurrentAction = NULL;
}

void UCCombatComponent::OnCollision()
{
	CheckTrue(CurrentCombatIndex == -1);
	CheckFalse(Combats.Num() > CurrentCombatIndex);
	CheckNull(Combats[CurrentCombatIndex]);

	Combats[CurrentCombatIndex]->OnCollision();
}

void UCCombatComponent::OffCollision()
{
	CheckTrue(CurrentCombatIndex == -1);
	CheckFalse(Combats.Num() > CurrentCombatIndex);
	CheckNull(Combats[CurrentCombatIndex]);

	Combats[CurrentCombatIndex]->OffCollision();
}

int32 UCCombatComponent::GetSkillsCount()
{
	CheckTrueResult(CurrentCombatIndex == -1, 0);
	CheckFalseResult(Combats.Num() > CurrentCombatIndex, 0);
	CheckNullResult(Combats[CurrentCombatIndex], 0);

	return Combats[CurrentCombatIndex]->GetSkillActionCount();
}

float UCCombatComponent::GetSkillsCooldown(int32 InIndex)
{
	CheckTrueResult(CurrentCombatIndex == -1, 0);
	CheckFalseResult(Combats.Num() > CurrentCombatIndex, 0);
	CheckNullResult(Combats[CurrentCombatIndex], 0);

	UCAction* skill = Combats[CurrentCombatIndex]->GetSkillAction(InIndex);
	CheckNullResult(skill, 0);

	return skill->GetCooldown();
}

float UCCombatComponent::GetSkillsMaxCooldown(int32 InIndex)
{
	CheckTrueResult(CurrentCombatIndex == -1, 1.0f);
	CheckFalseResult(Combats.Num() > CurrentCombatIndex, 1.0f);
	CheckNullResult(Combats[CurrentCombatIndex], 1.0f);

	UCAction* skill = Combats[CurrentCombatIndex]->GetSkillAction(InIndex);
	CheckNullResult(skill, 1.0f);

	return skill->GetCooldownMax();
}

void UCCombatComponent::SwitchingMelee(int32 InCurrentIdx, int32 InNextIdx)
{
	Combats[InCurrentIdx]->UngrabWeapon();
	Combats[InCurrentIdx]->GetAttackAction()->End_Action();
	Combats[InNextIdx]->GrabWeapon();
	CurrentCombatIndex = InNextIdx;

	if (!!SwitchingSound)
		UGameplayStatics::PlaySound2D(GetWorld(), SwitchingSound);

	UCAction_MeleeCombo* melee = Cast<UCAction_MeleeCombo>(Combats[CurrentCombatIndex]->GetAttackAction());
	CheckNull(melee);

	CurrentAction = melee;

	if (State->IsStateAirAction())
		melee->MeleeAirAttack();
	else
		melee->MeleeAttack();
}

void UCCombatComponent::OnHitCharacter(AActor * DamagedActor, AController * EventInstigator, AActor * DamageCauser)
{
	if(!!CurrentAction)
		CurrentAction->HitCharacter(DamagedActor, EventInstigator, DamageCauser);

	CheckTrue(CurrentCombatIndex == -1);
	CheckFalse(Combats.Num() > CurrentCombatIndex);
	CheckNull(Combats[CurrentCombatIndex]);
	Combats[CurrentCombatIndex]->GetWeapon()->HitCharacter(DamagedActor, EventInstigator, DamageCauser);
}

void UCCombatComponent::ClearDelegates()
{
	if(OnGrabWeaponDelegate.IsBound())
		OnGrabWeaponDelegate.Clear();

	if(OnUngrabWeaponDelegate.IsBound())
		OnUngrabWeaponDelegate.Clear();

	if (OnEndEquipDelegate.IsBound())
		OnEndEquipDelegate.Clear();

	if (OnEndSheathDelegate.IsBound())
		OnEndSheathDelegate.Clear();

	if (OnSwitchingDelegate.IsBound())
		OnSwitchingDelegate.Clear();

}
