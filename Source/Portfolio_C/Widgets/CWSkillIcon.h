#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWSkillIcon.generated.h"

UCLASS()
class PORTFOLIO_C_API UCWSkillIcon : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateProgressBar(float InValue, float InMaxValue);
};
