#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CBTTaskNode_Flee.generated.h"

UCLASS()
class PORTFOLIO_C_API UCBTTaskNode_Flee : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector PlayerKey;

	UPROPERTY(EditAnywhere)
		float Acceptance = 50.f;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebug = EDrawDebugTrace::None;

	UPROPERTY(EditAnywhere)
		class UEnvQuery* Query;

public:
	UCBTTaskNode_Flee();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void HandleQueryResult(TSharedPtr<struct FEnvQueryResult> Result);

private:
	class AAIController* Controller;
	class UBehaviorTreeComponent* OwnerBT;
};
