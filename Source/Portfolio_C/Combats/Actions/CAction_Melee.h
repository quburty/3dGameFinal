#pragma once

#include "CoreMinimal.h"
#include "Combats/Actions/CAction.h"
#include "CAction_Melee.generated.h"

UCLASS()
class PORTFOLIO_C_API UCAction_Melee : public UCAction
{
	GENERATED_BODY()

public:
	bool Action() override;

public:
	void Begin_Action() override;
	void End_Action() override;

public:
	void HitCharacter
	(
		class AActor* DamagedActor,
		class AController* EventInstigator,
		class AActor* DamageCauser
	) override;

};
