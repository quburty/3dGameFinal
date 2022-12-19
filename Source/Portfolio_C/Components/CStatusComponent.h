#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

UENUM(BlueprintType)
enum class ESuperArmor : uint8
{
	None, Action, HealthRate, Max
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_C_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "UserSettings")
		float MaxHealth = 50.f;

	UPROPERTY(EditAnywhere, Category = "UserSettings")
		float WalkSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = "UserSettings")
		float RunSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = "UserSettings")
		ESuperArmor SuperArmor;

	UPROPERTY(EditAnywhere, Category = "UserSettings")
		float SuperArmorHpRate = 0.3f;

public:
	FORCEINLINE float GetHealthRate() { return Health / MaxHealth; }
	FORCEINLINE ESuperArmor GetSuperArmor() { return SuperArmor; }
	FORCEINLINE float GetSuperArmorHpRate() { return SuperArmorHpRate; }
	
public:	
	UCStatusComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintPure)
		float GetMaxHealth() { return MaxHealth; }

	UFUNCTION(BlueprintPure)
		float GetHealth() { return Health; }

	UFUNCTION(BlueprintPure)
		bool IsDead() { return bIsDead; }

	UFUNCTION(BlueprintPure)
		float GetWalkSpeed() { return WalkSpeed; }

	UFUNCTION(BlueprintPure)
		float GetRunSpeed() { return RunSpeed; }

public:
	void ReduceHealth(float InAmount);
	
	bool IsSuperArmorDamagedEnough();

private:
	float Health = 0.f;
	bool bIsDead = false;

	float SuperArmorDamagedHealth = 0.f;
};
