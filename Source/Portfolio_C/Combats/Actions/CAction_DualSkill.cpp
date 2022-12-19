#include "CAction_DualSkill.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStateComponent.h"
#include "Etc/CPoison.h"

bool UCAction_DualSkill::Action()
{
	CheckFalseResult(Super::Action(), false);

	CheckNullResult(State, false);
	CheckFalseResult(State->IsStateIdle(), false);
	CheckFalseResult(AttackDatas.Num() > 0, false);

	const FAttackData& data = AttackDatas[0];
	Owner->PlayAnimMontage(data.MontageData.AnimMontage, data.MontageData.PlayRatio, data.MontageData.StartSection);
	data.MontageData.bCanMove ? State->SetMove() : State->SetStop();

	State->SetStateAction();

	bPoison = false;

	return true;
}

void UCAction_DualSkill::Begin_Action()
{
	Super::Begin_Action();

	bPoison = true;
}

void UCAction_DualSkill::End_Action()
{
	Super::End_Action();

	bPoison = false;

	State->SetStateIdle();
	State->SetMove();
}

void UCAction_DualSkill::HitCharacter(AActor * DamagedActor, AController * EventInstigator, AActor * DamageCauser)
{
	Super::HitCharacter(DamagedActor, EventInstigator, DamageCauser);

	if (bPoison)
	{
		ACharacter* character = Cast<ACharacter>(DamagedActor);
		CheckNull(character);

		ACPoison* poison = GetWorld()->SpawnActorDeferred<ACPoison>(ACPoison::StaticClass(), FTransform(), DamagedActor);
		poison->AttachToActor(character, 
			FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		poison->FinishSpawning(FTransform());
	}
}
