#include "CBTTaskNode_Attack.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CCombatComponent.h"
#include "Components/CStateComponent.h"
#include "AIController.h"

UCBTTaskNode_Attack::UCBTTaskNode_Attack()
{
	NodeName = "Attack";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* controller = Cast<AAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Succeeded);

	Owner = Cast<ACharacter>(controller->GetPawn());
	CheckNullResult(Owner, EBTNodeResult::Succeeded);

	UCCombatComponent* combat = Cast<UCCombatComponent>(
		Owner->GetComponentByClass(UCCombatComponent::StaticClass()));

	CheckNullResult(combat, EBTNodeResult::Succeeded);
	
	combat->Attack();

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UCBTTaskNode_Attack::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	AAIController* controller = Cast<AAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Aborted);

	Owner = Cast<ACharacter>(controller->GetPawn());
	CheckNullResult(Owner, EBTNodeResult::Aborted);

	UCCombatComponent* combat = Cast<UCCombatComponent>(
		Owner->GetComponentByClass(UCCombatComponent::StaticClass()));

	CheckNullResult(combat, EBTNodeResult::Aborted);

	combat->EndAction();

	return EBTNodeResult::Aborted;
}

void UCBTTaskNode_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UCStateComponent* state = Cast<UCStateComponent>(Owner->GetComponentByClass(UCStateComponent::StaticClass()));
	CheckNull(state);

	if (state->IsStateIdle())
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

}