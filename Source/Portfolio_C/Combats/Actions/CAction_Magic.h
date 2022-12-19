#pragma once

#include "CoreMinimal.h"
#include "Combats/Actions/CAction.h"
#include "CAction_Magic.generated.h"

UCLASS(Blueprintable)
class PORTFOLIO_C_API UCAction_Magic : public UCAction
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACThrow> ThrowClass;

	UPROPERTY(EditDefaultsOnly)
		float ThrowSpeed = 100.f;

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
};
