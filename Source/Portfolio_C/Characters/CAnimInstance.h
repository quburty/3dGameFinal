#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CStateComponent.h"
#include "Components/CIKComponent.h"
#include "CAnimInstance.generated.h"

UCLASS()
class PORTFOLIO_C_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	FORCEINLINE float GetSpeed() const { return Speed; }
	FORCEINLINE float GetDirection() const { return Direction; }

protected:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaTimeX) override;

protected:
	UPROPERTY(BlueprintReadOnly)
		bool bIsCombatMode = false;

	UPROPERTY(BlueprintReadOnly)
		int32 CombatModeIndex = 0;

	UPROPERTY(BlueprintReadOnly)
		bool bOnAir = false;

	UPROPERTY(BlueprintReadOnly)
		float Speed = 0.f;

	UPROPERTY(BlueprintReadOnly)
		float Direction = 0.f;

	UPROPERTY(BlueprintReadOnly)
		bool bIsParkour;

	UPROPERTY(BlueprintReadOnly)
		EStateType StateType;
	
	UPROPERTY(BlueprintReadOnly)
		FVector ParkourVelocity;

	UPROPERTY(BlueprintReadOnly)
		FIKData IKFeetData;

private:
	class ACharacter* Character;
	
};
