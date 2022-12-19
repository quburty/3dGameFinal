#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterInterface.h"
#include "GenericTeamAgentInterface.h"
#include "DataAssets/DataAsset_Montages.h"
#include "CPlayer.generated.h"

UCLASS()
class PORTFOLIO_C_API ACPlayer : public ACharacter, public ICharacterInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ACPlayer();

public:
	FORCEINLINE class UCMontagesComponent* GetMontage() override { return Montage; }
	FORCEINLINE class UCStateComponent* GetState() override { return State; }
	FORCEINLINE class UCCombatComponent* GetCombat() { return Combat; }

protected:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		class UCMontagesComponent* Montage;

	UPROPERTY(VisibleAnywhere)
		class UCCombatComponent* Combat;

	UPROPERTY(VisibleAnywhere)
		class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleAnywhere)
		class UCTargetComponent* Target;

	UPROPERTY(VisibleAnywhere)
		class UCParkourComponent* Parkour;

	UPROPERTY(VisibleAnywhere)
		class UCIKComponent* IK;

private:
	UFUNCTION(BlueprintCallable)
		void SetInvincibility();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual FGenericTeamId GetGenericTeamId() const override;

public:
	void Roll();
	void Backstep();

	void SetRotationLock();
	void SetRotationFree();

	void OnWeapon(uint8 InWeaponIndex);
	void Attack();

	void Skill(int32 InIndex);

	void Targetting();
	void TargetChange(bool InRight);

	void BeginPostProcess(class UMaterialInstance* InPostProcess);
	void EndPostProcess();

	UFUNCTION(BlueprintCallable)
		void Hook();
	
public:
	void Hook_Begin();
	void Hook_End();

private:
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator,
		AActor * DamageCauser
	) override;

	void Hitted();
	void Dead() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		TSubclassOf<class ACHook> HookActor;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		FAssetMontageData HookMontage;

private:
	class UMaterialInstance* PostProcess;

	bool bInvincibility; //Only for test

};
