#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "DataAssets/DataAsset_Montages.h"
#include "CEnemy_AI_Boss.generated.h"

USTRUCT(BlueprintType)
struct FPhaseData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FAssetMontageData PhaseChangeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float NextPhaseHpRate = 0.5f;
};

UCLASS()
class PORTFOLIO_C_API ACEnemy_AI_Boss : public ACEnemy
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		class UCCombatComponent* Combat;

private:
	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		TArray<FPhaseData> PhaseDatas;

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void PhaseChange();
	void PhaseChange_Implementation();

public:
	ACEnemy_AI_Boss();

protected:
	virtual void BeginPlay() override;

public:
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator,
		AActor * DamageCauser
	) override;

	//virtual class UBehaviorTree* GetBehaviorTree() override;
	bool IsAlwaysPerceptPlayer() override { return true; }

private:
	uint8 Phase;
};
