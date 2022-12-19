#pragma once

#include "CoreMinimal.h"
#include "Combats/Actions/CAction.h"
#include "CAction_SwordSkill.generated.h"

UCLASS()
class PORTFOLIO_C_API UCAction_SwordSkill : public UCAction
{
	GENERATED_BODY()
	
public:
	bool Action() override;
	void Begin_Action() override;
	void End_Action() override;
};
