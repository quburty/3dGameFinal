#pragma once

#include "CoreMinimal.h"
#include "Combats/Actions/CAction.h"
#include "CAction_MeleeCombo.generated.h"

UCLASS()
class PORTFOLIO_C_API UCAction_MeleeCombo : public UCAction
{
	GENERATED_BODY()

public:
	bool Action() override;

	void MeleeAttack();
	void MeleeAirAttack();

public:
	void Begin_Action() override;
	void End_Action() override;

	void Begin_EnableCombo();
	void End_EnableCombo();

private:
	void Begin_AirAction();
	void End_AirAction();

public:
	void HitCharacter
	(
		class AActor* DamagedActor,
		class AController* EventInstigator,
		class AActor* DamageCauser
	) override;

	FORCEINLINE bool IsComboExist() { return bExist; }
	FORCEINLINE bool IsComboEnable() { return bEnable; }
	FORCEINLINE void SetPostEffect(class UMaterialInstance* InPostEffect) { PostEffect = InPostEffect; }

private:
	class UMaterialInstance* PostEffect;

	bool bExist = false;
	bool bEnable = false;
	
};
