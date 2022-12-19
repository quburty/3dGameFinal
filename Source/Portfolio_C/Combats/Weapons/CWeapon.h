#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnWeaponHitDelegate, class AActor*, DamagedActor, class AController*, EventInstigator, class AActor*, DamageCauser);

UCLASS()
class PORTFOLIO_C_API ACWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ACWeapon();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintNativeEvent)
		void GrabWeapon();
	virtual void GrabWeapon_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void UngrabWeapon();
	virtual void UngrabWeapon_Implementation();

protected:
	UFUNCTION(BlueprintPure, BlueprintCallable)
		class ACharacter* GetOwnerCharacter();

protected:
	UFUNCTION()
		virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult &SweepResult);

public:
	UFUNCTION()
		void OnCollision();

	UFUNCTION()
		void OffCollision();

	UFUNCTION()
		virtual void HitCharacter
		(
			class AActor* DamagedActor,
			class AController* EventInstigator,
			class AActor* DamageCauser
		); //No association with UCAction::HitCharacter()

protected:
	UFUNCTION(BlueprintCallable)
		void AttachToSocket(FName SocketName);

public:
	FOnWeaponHitDelegate OnWeaponHitDelegate;

protected:
	class ACharacter* Owner;
	TArray<UShapeComponent*> Colliders;

	TSet<ACharacter* > HittedCharacters;
};
