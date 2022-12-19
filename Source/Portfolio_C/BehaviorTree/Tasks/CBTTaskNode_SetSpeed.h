#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_SetSpeed.generated.h"

UENUM(BlueprintType)
enum class ESpeedType : uint8
{
	Walk, Run, Max,
};

UCLASS()
class PORTFOLIO_C_API UCBTTaskNode_SetSpeed : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		ESpeedType SpeedType = ESpeedType::Walk;

public:
	UCBTTaskNode_SetSpeed();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
