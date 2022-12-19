#include "Characters/CEnemy_AI_Boss.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CCombatComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CStateComponent.h"
#include "Controller/CAIController.h"

ACEnemy_AI_Boss::ACEnemy_AI_Boss()
	:Super()
{
	Combat = CreateDefaultSubobject<UCCombatComponent>("Combat");

}

void ACEnemy_AI_Boss::PhaseChange_Implementation()
{
	CheckFalse(Phase < PhaseDatas.Num());
	CheckNull(State);

	ACAIController* controller = Cast<ACAIController>(GetController());
	if(!!controller)
		controller->SetBehaviorTree(PhaseDatas[Phase].BehaviorTree);
	
	const FAssetMontageData& data = PhaseDatas[Phase].PhaseChangeMontage;
	CheckNull(data.AnimMontage);

	PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);
	State->SetStateAction();
	if (data.bCanMove == false)
	{
		State->SetStop();
	}

}

void ACEnemy_AI_Boss::BeginPlay()
{
	Super::BeginPlay();

	Phase = 0;
}

float ACEnemy_AI_Boss::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float value = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (Phase < PhaseDatas.Num() &&
		PhaseDatas[Phase].NextPhaseHpRate > Status->GetHealthRate())
	{
		Phase++;
		PhaseChange();
	}

	return value;
}

//UBehaviorTree* ACEnemy_AI_Boss::GetBehaviorTree()
//{
//	CheckFalseResult(Phase < PhaseDatas.Num(), NULL);
//	return PhaseDatas[Phase].BehaviorTree;
//}