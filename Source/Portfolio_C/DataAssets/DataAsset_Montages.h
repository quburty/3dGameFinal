#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_Montages.generated.h"

USTRUCT(BlueprintType)
struct FAssetMontageData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName StartSection = "None";

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bCanMove = false;
};

UCLASS()
class PORTFOLIO_C_API UDataAsset_Montages : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FORCEINLINE const FAssetMontageData* GetHitMontageData() const { return &HitMontageData; }
	FORCEINLINE const FAssetMontageData* GetDieMontageData() const { return &DieMontageData; }
	FORCEINLINE const FAssetMontageData* GetRollMontageData() const { return &RollMontageData; }
	FORCEINLINE const FAssetMontageData* GetBackstepMontageData() const { return &BackstepMontageData; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FAssetMontageData HitMontageData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FAssetMontageData DieMontageData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FAssetMontageData RollMontageData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FAssetMontageData BackstepMontageData;
};
