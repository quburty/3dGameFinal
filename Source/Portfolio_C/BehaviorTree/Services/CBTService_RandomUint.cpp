#include "CBTService_RandomUint.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"

void UCBTService_RandomUint::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	UBlackboardComponent* blackboard = SearchData.OwnerComp.GetBlackboardComponent();
	CheckNull(blackboard);
	
	int32 random = UKismetMathLibrary::RandomIntegerInRange(Min, Max);

	blackboard->SetValueAsInt(UintKey.SelectedKeyName, random);
}