#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterInterface.h"
#include "Engine/EngineTypes.h"
#include "CEnemy.generated.h"

UENUM()
enum class EHealthBarType : uint8
{
	Default, Boss, Max
};

UCLASS()
class PORTFOLIO_C_API ACEnemy : public ACharacter, public ICharacterInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
		class UCMontagesComponent* Montage;
	
	UPROPERTY(VisibleAnywhere)
		class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* HealthBarWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		EHealthBarType HealthBarType = EHealthBarType::Default;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		class UParticleSystem* HittedEffect;

public:
	class UCMontagesComponent* GetMontage() override { return Montage; }
	class UCStateComponent* GetState() override { return State; }
	class UCStatusComponent* GetStatus() override { return Status; }
	//class UWidgetComponent* GetHealthBarWidget() { return HealthBarWidget; }

public:
	ACEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintNativeEvent)
		void Hitted();
	void Hitted_Implementation();

public:
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const & DamageEvent,
		class AController * EventInstigator,
		AActor * DamageCauser
	) override;
	
private:
	void Dead() override;

private:
	FTimerHandle HealthBarHandle;
};
