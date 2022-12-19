#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMulticastDelegate);
DECLARE_MULTICAST_DELEGATE(FSwitchingDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTickDelegate, float, DeltaTime);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PORTFOLIO_C_API UCCombatComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		TArray<class UDataAsset_Combat *> DataAssets;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		int32 StartCombat = -1;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		class USoundBase* SwitchingSound;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		class UMaterialInstance* AirActionPostEffect;

public:
	UCCombatComponent();
	
public:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

public:
	void Attack();
	void Skill(int32 InSkillIndex = 0);

	UFUNCTION(BlueprintCallable)
		void OnWeapon(int32 InCombatIndex);

	UFUNCTION()
		void Sheath();

	void GrabWeapon();
	void UngrabWeapon();

	void EndEquip();
	void EndSheath();

	bool IsEquipped();

	void OnEnableCombo();
	void OffEnableCombo();

	void BeginAction();

	void EndAction();

	void OnCollision();
	void OffCollision();

	FORCEINLINE int32 GetCurrentCombatIndex() { return CurrentCombatIndex; }
	
	int32 GetSkillsCount();
	float GetSkillsCooldown(int32 InIndex);
	float GetSkillsMaxCooldown(int32 InIndex);


private:
	UFUNCTION()
		void SwitchingMelee(int32 InCurrentIdx, int32 InNextIdx);

	UFUNCTION()
		void OnHitCharacter
		(
			class AActor * DamagedActor,
			class AController * EventInstigator,
			class AActor * DamageCauser
		);

	void ClearDelegates();

private:
	ACharacter* Owner;
	class UCStateComponent* State;

	TArray<class UCCombat*> Combats;
	int32 CurrentCombatIndex = -1; //-1 : No Combat
	class UCAction* CurrentAction;

	FMulticastDelegate OnGrabWeaponDelegate;
	FMulticastDelegate OnUngrabWeaponDelegate;
	FMulticastDelegate OnEndEquipDelegate;
	FMulticastDelegate OnEndSheathDelegate;

	FSwitchingDelegate OnSwitchingDelegate;

	FTickDelegate OnTickDelegate;
};
