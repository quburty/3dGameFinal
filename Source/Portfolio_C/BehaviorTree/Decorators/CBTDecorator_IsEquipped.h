#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTDecorator_IsEquipped.generated.h"

UCLASS()
class PORTFOLIO_C_API UCBTDecorator_IsEquipped : public UBTDecorator
{
	GENERATED_BODY()

private:
	UCBTDecorator_IsEquipped();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
