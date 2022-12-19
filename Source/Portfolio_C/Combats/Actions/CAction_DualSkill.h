#pragma once

#include "CoreMinimal.h"
#include "Combats/Actions/CAction.h"
#include "CAction_DualSkill.generated.h"

UCLASS()
class PORTFOLIO_C_API UCAction_DualSkill : public UCAction
{
	GENERATED_BODY()
	
public:
	bool Action() override;
	void Begin_Action() override;
	void End_Action() override;

public:
	void HitCharacter
	(
		class AActor* DamagedActor,
		class AController* EventInstigator,
		class AActor* DamageCauser
	) override;

private:
	bool bPoison = false;

};
