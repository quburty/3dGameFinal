#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTDecorator_Health.generated.h"

UCLASS()
class PORTFOLIO_C_API UCBTDecorator_Health : public UBTDecorator
{
	GENERATED_BODY()

private:
	UCBTDecorator_Health();
	
private:
	UPROPERTY(EditAnywhere)
		float Rate = 0.5f;

	UPROPERTY(EditAnywhere)
		bool Over = false;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
