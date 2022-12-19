#include "CBTDecorator_IsEquipped.h"
#include "Global.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Components/CCombatComponent.h"

UCBTDecorator_IsEquipped::UCBTDecorator_IsEquipped()
{
	NodeName = "IsEquipped";
}

bool UCBTDecorator_IsEquipped::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AAIController* controller = Cast<AAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, false);

	ACharacter* owner = Cast<ACharacter>(controller->GetPawn());
	CheckNullResult(owner, false);

	UCCombatComponent* combat = CHelpers::GetComponent<UCCombatComponent>(owner);
	CheckNullResult(combat, false);

	return combat->IsEquipped();
}