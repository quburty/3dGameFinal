#include "CAction_SpearSkill.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

bool UCAction_SpearSkill::Action()
{
	CheckFalseResult(Super::Action(), false);

	CheckNullResult(State, false);
	CheckFalseResult(State->IsStateIdle(),false);
	CheckFalseResult(AttackDatas.Num() > 0,false);

	const FAttackData& data = AttackDatas[0];
	Owner->PlayAnimMontage(data.MontageData.AnimMontage, data.MontageData.PlayRatio, data.MontageData.StartSection);
	data.MontageData.bCanMove ? State->SetMove() : State->SetStop();

	HittedCharacters.Empty();

	State->SetStateAction();

	return true;
}

void UCAction_SpearSkill::Begin_Action()
{
	Super::Begin_Action();

	for (ACharacter* hittedCharacter : HittedCharacters)
		hittedCharacter->GetCharacterMovement()->Velocity = FVector(0);
}

void UCAction_SpearSkill::End_Action()
{
	Super::End_Action();
	
	State->SetStateIdle();
	State->SetMove();
}

void UCAction_SpearSkill::HitCharacter(AActor * DamagedActor, AController * EventInstigator, AActor * DamageCauser)
{
	Super::HitCharacter(DamagedActor, EventInstigator, DamageCauser);

	ACharacter* character = Cast<ACharacter>(DamagedActor);
	CheckNull(character);

	bool bFound = false;
	for (ACharacter* hittedCharacter : HittedCharacters)
	{
		if (character == hittedCharacter)
		{
			bFound = true;
			break;
		}
	}

	CheckTrue(bFound);

	AAIController* controller = Cast<AAIController>(character->GetController());
	if (!!controller)
	{
		controller->StopMovement();
	}

	FVector normal = character->GetActorLocation() - Owner->GetActorLocation();
	normal = normal.GetSafeNormal2D();

	character->LaunchCharacter(normal*1000.f, false, false);

}