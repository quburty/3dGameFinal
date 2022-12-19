#include "Combats/Actions/CAction_Magic.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Etc/CThrow.h"

bool UCAction_Magic::Action()
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

void UCAction_Magic::Begin_Action()
{
	Super::Begin_Action();

	CheckNull(Owner);
	ACThrow* fireBall = GetWorld()->SpawnActorDeferred<ACThrow>(ThrowClass, Owner->GetTransform(), Owner);
	fireBall->FinishSpawning(Owner->GetTransform());
	fireBall->SetProjectileVelocity(Owner->GetActorRotation().Vector() * ThrowSpeed);

}

void UCAction_Magic::End_Action()
{
	Super::End_Action();

	State->SetStateIdle();
	State->SetMove();
}

void UCAction_Magic::HitCharacter(AActor * DamagedActor, AController * EventInstigator, AActor * DamageCauser)
{
	Super::HitCharacter(DamagedActor, EventInstigator, DamageCauser);


}
