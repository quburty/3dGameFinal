#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_RandomUint.generated.h"

UCLASS()
class PORTFOLIO_C_API UCBTService_RandomUint : public UBTService
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector UintKey;

	UPROPERTY(EditAnywhere)
		uint8 Max = 1;

	UPROPERTY(EditAnywhere)
		uint8 Min = 0;

protected:
	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;

};
