#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Sheath.generated.h"

UCLASS()
class PORTFOLIO_C_API UCBTTaskNode_Sheath : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_Sheath();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	class ACharacter* Owner;

};
