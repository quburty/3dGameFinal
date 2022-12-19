#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DataAssets/DataAsset_Montages.h"
#include "CParkourComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_C_API UCParkourComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		float ParkourCheckDistance = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		float ParkourCheckHeight = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		float ZOffset = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		float HandsZOffset = 50.0f;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		TEnumAsByte<ETraceTypeQuery> TraceQuery;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		bool bDrawDebugLine;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		float ParkourSpeed = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		float ParkourJumpCheckWidth = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		FAssetMontageData ParkourJumpLeft;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		FAssetMontageData ParkourJumpRight;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		FAssetMontageData ParkourCornerLeft;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		FAssetMontageData ParkourCornerRight;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		FAssetMontageData ClimbUp;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		float JumpImpulseRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		FVector MeshAdjust;

public:
	FORCEINLINE FVector GetParkourVelocity() { return ParkourVelocity; }
	FORCEINLINE float GetJumpImpulseRate() { return JumpImpulseRate; }

public:	
	UCParkourComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void ParkourLeft(float DeltaTime);
	void ParkourRight(float DeltaTime);

	void ParkourLeftJump();
	void ParkourRightJump();

	void ParkourClimbUp();

	void EndParkourAction();

private:
	void ParkourTick(float DeltaTime);

private:
	void Begin_Parkour();

	void End_Parkour();

	void ParkourCornerLeftJump();
	void ParkourCornerRightJump();

private:
	bool IsCanParkour(FVector InCharacterLocation, FVector InCharacterForwardVector);
	bool IsCanClimbUp();

private:
	class ACharacter* OwnerCharacter;
	class UCStateComponent* State;

	bool bParkour;

private:
	FVector ParkourLocation;
	FRotator ParkourRotation;

	bool bClimbUp;

private:
	FVector ParkourVelocity;

	FVector PrevCharacterLocation;
};