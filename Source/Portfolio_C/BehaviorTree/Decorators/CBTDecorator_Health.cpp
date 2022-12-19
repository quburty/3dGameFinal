#include "CBTDecorator_Health.h"
#include "Global.h"
#include "AIController.h"
#include "Components/CStatusComponent.h"
#include "GameFramework/Character.h"

UCBTDecorator_Health::UCBTDecorator_Health()
{
	NodeName = "Health";
}

bool UCBTDecorator_Health::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AAIController* controller = Cast<AAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, true);

	ACharacter* owner = Cast<ACharacter>(controller->GetPawn());
	CheckNullResult(owner, true);

	UCStatusComponent* status = CHelpers::GetComponent<UCStatusComponent>(owner);
	CheckNullResult(status, true);

	float hpRate = status->GetHealth() / status->GetMaxHealth();

	if (Over)
		return hpRate > Rate ? true : false;
	else
		return hpRate < Rate ? true : false;
}