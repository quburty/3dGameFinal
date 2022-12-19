#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Jump UMETA(DisplayName = "Jump"),
	Action UMETA(DisplayName = "Action"),
	AirAction UMETA(DisplayName = "AirAction"),
	Hitted UMETA(DisplayName = "Hitted"),
	Parkour UMETA(DisplayName = "Parkour"),
	Hook UMETA(DisplayName = "Hook"),
	Dead UMETA(DisplayName = "Dead"),
	Max UMETA(DisplayName = "Max(ERROR)")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_C_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCStateComponent();

public:
	FORCEINLINE EStateType GetStateType() { return Type; }

public:
	FORCEINLINE bool IsStateIdle() { return Type == EStateType::Idle; }
	FORCEINLINE bool IsStateJump() { return Type == EStateType::Jump; }
	FORCEINLINE bool IsStateAction() { return Type == EStateType::Action; }
	FORCEINLINE bool IsStateAirAction() { return Type == EStateType::AirAction; }
	FORCEINLINE bool IsStateHitted() { return Type == EStateType::Hitted; }
	FORCEINLINE bool IsStateParkour() { return Type == EStateType::Parkour; }
	FORCEINLINE bool IsStateHook() { return Type == EStateType::Hook; }
	FORCEINLINE bool IsStateDead() { return Type == EStateType::Dead; }

	FORCEINLINE void SetStateIdle() { Type = EStateType::Idle; }
	FORCEINLINE void SetStateAction() { Type = EStateType::Action; }
	FORCEINLINE void SetStateAirAction() { Type = EStateType::AirAction; }
	FORCEINLINE void SetStateHitted() { Type = EStateType::Hitted; }
	FORCEINLINE void SetStateParkour() { Type = EStateType::Parkour; }
	FORCEINLINE void SetStateHook() { Type = EStateType::Hook; }

private:
	FORCEINLINE void SetStateJump() { Type = EStateType::Jump; }
	FORCEINLINE void SetStateDead() { Type = EStateType::Dead; }

public:
	FORCEINLINE bool IsCanMove() { return bCanMove; }

	FORCEINLINE void SetMove() { bCanMove = true; }
	FORCEINLINE void SetStop() { bCanMove = false; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintPure)
		const EStateType GetStateUEnum() { return Type; }

private:
	EStateType Type = EStateType::Idle;
	bool bCanMove = true;
};
