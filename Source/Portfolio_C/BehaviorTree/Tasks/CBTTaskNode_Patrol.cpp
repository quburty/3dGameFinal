#include "CBTTaskNode_Patrol.h"
#include "Global.h"
#include "AIController.h"
#include "Characters/CEnemy.h"
#include "Components/CPatrolComponent.h"

UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	Controller = Cast<AAIController>(OwnerComp.GetOwner());
	CheckNullResult(Controller, EBTNodeResult::Succeeded);

	ACEnemy* enemy = Cast<ACEnemy>(Controller->GetPawn());
	CheckNullResult(enemy, EBTNodeResult::Succeeded);

	Patrol = CHelpers::GetComponent<UCPatrolComponent>(enemy);
	CheckNullResult(Patrol, EBTNodeResult::Succeeded);

	Patrol->UpdateNextIndex();

	FVector location;
	float accept;

	if (Patrol->GetMoveTo(location, accept) == false)
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UCBTTaskNode_Patrol::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	Controller->StopMovement();

	return EBTNodeResult::Aborted;
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FVector location;
	float acceptance;

	Patrol->GetMoveTo(location, acceptance);
	EPathFollowingRequestResult::Type type = Controller->MoveToLocation(location, acceptance, false);

	if (type == EPathFollowingRequestResult::Failed)
	{
		//Patrol->UpdateNextIndex();

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	if (type == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//Patrol->UpdateNextIndex();

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}