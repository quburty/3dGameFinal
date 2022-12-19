#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDestructible.generated.h"

UCLASS()
class PORTFOLIO_C_API ACDestructible : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly)
		class UDestructibleComponent* Destructible;

private:
	UPROPERTY(EditAnywhere, Category = "UserSettings")
		float MaxHp;

public:
	ACDestructible();

protected:
	virtual void BeginPlay() override;

public:
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

private:
	float CurrentHp;
};
