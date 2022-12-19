#include "CBTTaskNode_SetSpeed.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Components/CStatusComponent.h"

UCBTTaskNode_SetSpeed::UCBTTaskNode_SetSpeed()
{
	NodeName = "SetSpeed";
}

EBTNodeResult::Type UCBTTaskNode_SetSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* controller = Cast<AAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Succeeded);

	ACharacter* owner = Cast<ACharacter>(controller->GetPawn());
	CheckNullResult(owner, EBTNodeResult::Succeeded);

	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(owner);
	CheckNullResult(status, EBTNodeResult::Succeeded);

	switch (SpeedType)
	{
	case ESpeedType::Walk: owner->GetCharacterMovement()->MaxWalkSpeed = status->GetWalkSpeed(); break;
	case ESpeedType::Run: owner->GetCharacterMovement()->MaxWalkSpeed = status->GetRunSpeed(); break;
	}

	return EBTNodeResult::Succeeded;
}