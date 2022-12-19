#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CThrow.generated.h"

UCLASS()
class PORTFOLIO_C_API ACThrow : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleDefaultsOnly)
		class UParticleSystemComponent* Particle;

	UPROPERTY(VisibleDefaultsOnly)
		class UProjectileMovementComponent* Projectile;

	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		class UParticleSystem* HitEffect;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		float Damage = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		float LifeTime = 5.0f;

public:	
	ACThrow();

public:
	void SetProjectileVelocity(FVector InVelocity);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

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
	float LeftTime;
};
