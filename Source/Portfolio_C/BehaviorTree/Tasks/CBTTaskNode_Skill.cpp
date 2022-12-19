#include "CBTTaskNode_Skill.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Components/CCombatComponent.h"
#include "Components/CStateComponent.h"
#include "AIController.h"

UCBTTaskNode_Skill::UCBTTaskNode_Skill()
{
	NodeName = "Skill";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Skill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* controller = Cast<AAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Succeeded);

	Owner = Cast<ACharacter>(controller->GetPawn());
	CheckNullResult(Owner, EBTNodeResult::Succeeded);

	UCCombatComponent* combat = Cast<UCCombatComponent>(
		Owner->GetComponentByClass(UCCombatComponent::StaticClass()));

	CheckNullResult(combat, EBTNodeResult::Succeeded);
	
	int32 index = 0;

	if (bUseKey)
	{
		UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
		CheckNullResult(blackboard, EBTNodeResult::Succeeded);
		index = blackboard->GetValueAsInt(PatternIntKey.SelectedKeyName);
	}
	else
		index = SkillIndex;

	combat->Skill(index);

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UCBTTaskNode_Skill::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
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

void UCBTTaskNode_Skill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UCStateComponent* state = Cast<UCStateComponent>(Owner->GetComponentByClass(UCStateComponent::StaticClass()));
	CheckNull(state);

	if (state->IsStateIdle())
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}