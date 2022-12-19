#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAssets/DataAsset_Montages.h"
#include "DataAsset_Combat.generated.h"

USTRUCT(BlueprintType)
struct FAttackData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FAssetMontageData MontageData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Power = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float HitStop = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float HitStopSpeed = 0.001f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UParticleSystem* HitEffect;
};

USTRUCT(BlueprintType)
struct FAttackData_Skill : public FAttackData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<class UCAction> SkillActionClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bUseCooldown;
};

UCLASS()
class PORTFOLIO_C_API UDataAsset_Combat : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FORCEINLINE TSubclassOf<class ACWeapon> GetWeaponClass() { return WeaponClass; }
	FORCEINLINE FAssetMontageData GetEquipMontageData() { return EquipMontageData; }
	FORCEINLINE FAssetMontageData GetSheathMontageData() { return SheathMontageData; }
	//FORCEINLINE TSubclassOf<class UCAction> GetActionClass() { return ActionClass; }
	FORCEINLINE TArray<FAttackData> GetAttackDatas() { return AttackDatas; }
	FORCEINLINE const TArray<FAttackData_Skill>& GetSkillDatas() { return SkillDatas; }
	FORCEINLINE bool GetLockRotation() { return bLockRotation; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<class ACWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bLockRotation = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FAssetMontageData EquipMontageData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FAssetMontageData SheathMontageData;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//	TSubclassOf<class UCAction> ActionClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FAttackData> AttackDatas;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FAttackData_Skill> SkillDatas;

};
