#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CHook.generated.h"

UCLASS()
class PORTFOLIO_C_API ACHook : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;

	UPROPERTY(VisibleDefaultsOnly)
		class USplineMeshComponent* Spline;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		class UParticleSystem* HitEffect;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		float LifeTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		float FlySpeed = 500.0f;

public:
	ACHook();

public:
	void SetProjectileDirection(FVector InDirection);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
		void BeginOverlap
		(
			class UPrimitiveComponent * OverlappedComponent,
			class AActor * OtherActor,
			class UPrimitiveComponent * OtherComp,
			int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult & SweepResult
		);

private:
	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		bool bUseSpline = true;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		FName HandSocket = "Name_NONE";

	class UCharacterMovementComponent* Movement;
	class UCStateComponent* State;

	bool bHitted;
	float LeftTime;
};
