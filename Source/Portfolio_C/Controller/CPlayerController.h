#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

UCLASS()
class PORTFOLIO_C_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleDefaultsOnly)
		class UInputComponent* Input;

public:
	ACPlayerController();

private:
	void BeginPlay() override;
	void OnPossess(APawn* InPawn) override;
	void SetupInputComponent() override;

private:
	void OnJump();
	//void OnRoll();

	void OnMoveForward(float axis);
	void OnMoveRight(float axis);

	void OnHorizontalLook(float axis);
	void OnVerticalLook(float axis);

	void OnSword();
	void OnDual();
	void OnSpear();
	void OnMagic();

	void OnAttack();

	void OnSkill1() { OnSkill(0); }
	void OnSkill2() { OnSkill(1); }
	void OnSkill3() { OnSkill(2); }

	void OnSkill(int32 InIndex);

	void OnTargetting();
	void OnTargetChangeLeft();
	void OnTargetChangeRight();

	void OnExit();

private:
	class ACPlayer* Player;

	class UCParkourComponent* Parkour;
};
