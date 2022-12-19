#pragma once

#include "CoreMinimal.h"
#include "Combats/Weapons/CWeapon.h"
#include "CWeaponMagic.generated.h"

UCLASS()
class PORTFOLIO_C_API ACWeaponMagic : public ACWeapon
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ACFire> FireClass;

	UPROPERTY(EditDefaultsOnly)
		float SpinSpeed = 1.0f;

	UPROPERTY(EditDefaultsOnly)
		float SpinRadius = 100.0f;

	UPROPERTY(EditDefaultsOnly)
		uint32 FireCount = 3;

	UPROPERTY(EditDefaultsOnly)
		float HitTimeInterval = 0.5f;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void GrabWeapon_Implementation() override;

	virtual void UngrabWeapon_Implementation() override;

protected:
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult) override;

	UFUNCTION()
		virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

public:
	virtual void HitCharacter
	(
		class AActor* DamagedActor,
		class AController* EventInstigator,
		class AActor* DamageCauser
	) override;

private:
	TArray<class ACFire*> Fires;
	TArray<class AActor*> TargetActors;

private:
	float Angle = 0.0f; //radian
	float Time = 0.0f;
};
