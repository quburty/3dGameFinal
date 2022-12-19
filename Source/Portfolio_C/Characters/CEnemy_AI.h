#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "CEnemy_AI.generated.h"

UCLASS()
class PORTFOLIO_C_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		uint8 TeamID = 1;

	UPROPERTY(EditAnywhere, Category = "UserSettings")
		bool bAlwaysPerceptPlayer;

public:
	FORCEINLINE bool IsAlwaysPerceptPlayer() { return bAlwaysPerceptPlayer; }

	virtual uint8 GetTeamID() override { return TeamID; }
	virtual class UBehaviorTree* GetBehaviorTree() override { return BehaviorTree; }

public:
	ACEnemy_AI();

public:
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator,
		AActor * DamageCauser
	) override;

};
