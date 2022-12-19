#include "CBTTaskNode_Sheath.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CCombatComponent.h"
#include "Components/CStateComponent.h"
#include "AIController.h"

UCBTTaskNode_Sheath::UCBTTaskNode_Sheath()
{
	NodeName = "Sheath";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Sheath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* controller = Cast<AAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Succeeded);

	Owner = Cast<ACharacter>(controller->GetPawn());
	CheckNullResult(Owner, EBTNodeResult::Succeeded);

	UCCombatComponent* combat = Cast<UCCombatComponent>(
		Owner->GetComponentByClass(UCCombatComponent::StaticClass()));

	CheckNullResult(combat, EBTNodeResult::Succeeded);

	if (combat->IsEquipped()==false)
		return EBTNodeResult::Succeeded;

	combat->Sheath();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Sheath::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UCStateComponent* state = Cast<UCStateComponent>(Owner->GetComponentByClass(UCStateComponent::StaticClass()));
	CheckNull(state);

	if (state->IsStateIdle())
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

}