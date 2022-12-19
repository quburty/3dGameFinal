#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Skill.generated.h"

UCLASS()
class PORTFOLIO_C_API UCBTTaskNode_Skill : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector PatternIntKey;

	UPROPERTY(EditAnywhere)
		int32 SkillIndex = 0;

	UPROPERTY(EditAnywhere)
		bool bUseKey;

public:
	UCBTTaskNode_Skill();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	class ACharacter* Owner;
};
