#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataAssets/DataAsset_Combat.h"
#include "CCombat.generated.h"

UCLASS()
class PORTFOLIO_C_API UCCombat : public UObject
{
	GENERATED_BODY()
	
public:	
	UCCombat();

public:
	UFUNCTION()
		void Equip();

	UFUNCTION()
		void Sheath();

	//FORCEINLINE bool IsEquipped() { return bEquipped; }

	FORCEINLINE void SetState(class UCStateComponent* InState) { State = InState; }
	FORCEINLINE void SetEquipData(const FAssetMontageData& InEquipMontageData) { EquipMontageData = InEquipMontageData; }
	FORCEINLINE void SetSheathData(const FAssetMontageData& InSheathMontageData) { SheathMontageData = InSheathMontageData; }
	FORCEINLINE void SetWeapon(class ACWeapon* InWeapon) { Weapon = InWeapon; }
	FORCEINLINE void SetLockControlRotate(bool InLock) { bLockControlRotate = InLock; }
	FORCEINLINE void SetAttackAction(class UCAction* const InAction) { AttackAction = InAction; }
	FORCEINLINE void AddSkillAction(class UCAction* const InAction) { SkillActions.Add(InAction); }

	FORCEINLINE class UCAction* GetAttackAction() { return AttackAction; }
	FORCEINLINE class ACWeapon* GetWeapon() { return Weapon; }
	class UCAction* GetSkillAction(int32 InIndex);
	FORCEINLINE int32 GetSkillActionCount() { return SkillActions.Num(); }

	UFUNCTION()
		void GrabWeapon();
	
	UFUNCTION()
		void UngrabWeapon();

	UFUNCTION()
		void EndEquip();

	UFUNCTION()
		void EndSheath();

	void OnEnableCombo();
	void OffEnableCombo();

	void OnCollision();
	void OffCollision();

	bool IsComboEnable();

private:
	class UCStateComponent* State;

	class ACWeapon* Weapon;
	FAssetMontageData EquipMontageData;
	FAssetMontageData SheathMontageData;
	bool bLockControlRotate;
	
	class UCAction* AttackAction;
	TArray<class UCAction *> SkillActions;

};
