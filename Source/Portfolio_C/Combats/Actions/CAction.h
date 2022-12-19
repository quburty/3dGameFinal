#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataAssets/DataAsset_Combat.h"
#include "CAction.generated.h"

UCLASS()
class PORTFOLIO_C_API UCAction : public UObject
{
	GENERATED_BODY()
	
public:	
	UCAction();

	virtual void PostInitProperties() override;

	UFUNCTION()
		void Tick(float DeltaTime);

public:
	UFUNCTION()
		virtual bool Action();

	UFUNCTION()
		virtual void Begin_Action() {}

	UFUNCTION()
		virtual void End_Action() {}

public:
	UFUNCTION()
		virtual void HitCharacter
		(
			class AActor* DamagedActor,
			class AController* EventInstigator,
			class AActor* DamageCauser
		);

private:
	UFUNCTION()
		void OnRestorDilation();

public:
	FORCEINLINE void SetAttackDatas(const TArray<FAttackData>& InAttackDatas) { AttackDatas = InAttackDatas; }
	FORCEINLINE void AddAttackData(const FAttackData& InAttackData) { AttackDatas.Add(InAttackData); }
	FORCEINLINE void SetUseCooldown(bool bUse) { UseCooldown = bUse; }
	FORCEINLINE float GetCooldown() { return Cooldown; }
	FORCEINLINE float GetCooldownMax() { return CooldownMax; }

protected:
	class ACharacter* Owner;
	class UCStateComponent* State;

	TArray<FAttackData> AttackDatas;

protected:
	int32 Index = 0;
	float Power = 0.f;

private:
	bool UseCooldown;

	float Cooldown = 0.f;
	float CooldownMax = 1.0f;
};
