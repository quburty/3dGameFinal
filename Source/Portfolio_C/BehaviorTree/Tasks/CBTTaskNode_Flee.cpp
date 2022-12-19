#include "CBTTaskNode_Flee.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "AIController.h"

UCBTTaskNode_Flee::UCBTTaskNode_Flee()
{
	NodeName = "Flee";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Flee::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerBT = &OwnerComp;

	Controller = Cast<AAIController>(OwnerComp.GetOwner());
	CheckNullResult(Controller, EBTNodeResult::Succeeded);

	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	AActor* target = Cast<AActor>(blackboard->GetValueAsObject(PlayerKey.SelectedKeyName));
	CheckNullResult(target, EBTNodeResult::Succeeded);

	CheckNullResult(Query, EBTNodeResult::Succeeded);
	FEnvQueryRequest query(Query,Controller->GetPawn());
	query.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &UCBTTaskNode_Flee::HandleQueryResult);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UCBTTaskNode_Flee::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	Controller->StopMovement();

	return EBTNodeResult::Aborted;
}

void UCBTTaskNode_Flee::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (Controller->GetMoveStatus() == EPathFollowingStatus::Idle)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UCBTTaskNode_Flee::HandleQueryResult(TSharedPtr<FEnvQueryResult> Result)
{
	if(Result.IsValid() == false)
		FinishLatentTask(*OwnerBT, EBTNodeResult::Failed);

	EPathFollowingRequestResult::Type result = Controller->MoveToLocation(Result->GetItemAsLocation(0), Acceptance);

	if (result == EPathFollowingRequestResult::Failed)
		FinishLatentTask(*OwnerBT, EBTNodeResult::Failed);

	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		FinishLatentTask(*OwnerBT, EBTNodeResult::Succeeded);
}
