#include "CPlayerController.h"
#include "Global.h"
#include "Components/InputComponent.h"
#include "Components/CParkourComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CTargetComponent.h"
#include "Characters/CPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

ACPlayerController::ACPlayerController()
{
	Input = CreateDefaultSubobject<UInputComponent>("Input");
	InputComponent = Input;
}

void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void ACPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Player = Cast<ACPlayer>(InPawn);

	CheckNull(Player);
	Parkour = CHelpers::GetComponent<UCParkourComponent>(Player);
}

void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	Input->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACPlayerController::OnJump);
	//Input->BindAction("Roll", EInputEvent::IE_Pressed, this, &ACPlayerController::OnRoll);

	Input->BindAxis("MoveForward", this, &ACPlayerController::OnMoveForward);
	Input->BindAxis("MoveRight", this, &ACPlayerController::OnMoveRight);

	Input->BindAxis("HorizontalLook", this, &ACPlayerController::OnHorizontalLook);
	Input->BindAxis("VerticalLook", this, &ACPlayerController::OnVerticalLook);

	Input->BindAction("Sword", EInputEvent::IE_Pressed, this, &ACPlayerController::OnSword);
	Input->BindAction("Dual", EInputEvent::IE_Pressed, this, &ACPlayerController::OnDual);
	Input->BindAction("Spear", EInputEvent::IE_Pressed, this, &ACPlayerController::OnSpear);
	Input->BindAction("Magic", EInputEvent::IE_Pressed, this, &ACPlayerController::OnMagic);

	Input->BindAction("Attack", EInputEvent::IE_Pressed, this, &ACPlayerController::OnAttack);
	Input->BindAction("Skill1", EInputEvent::IE_Pressed, this, &ACPlayerController::OnSkill1);
	Input->BindAction("Skill2", EInputEvent::IE_Pressed, this, &ACPlayerController::OnSkill2);
	Input->BindAction("Skill3", EInputEvent::IE_Pressed, this, &ACPlayerController::OnSkill3);
	
	Input->BindAction("Targetting", EInputEvent::IE_Pressed, this, &ACPlayerController::OnTargetting);
	Input->BindAction("TargetChangeLeft", EInputEvent::IE_Pressed, this, &ACPlayerController::OnTargetChangeLeft);
	Input->BindAction("TargetChangeRight", EInputEvent::IE_Pressed, this, &ACPlayerController::OnTargetChangeRight);

	Input->BindAction("Exit", EInputEvent::IE_Pressed, this, &ACPlayerController::OnExit);
}

void ACPlayerController::OnJump()
{
	UCTargetComponent* target = CHelpers::GetComponent<UCTargetComponent>(Player);
	if (!!target && target->IsTargeting())
	{
		if (Input->GetAxisValue("MoveForward") >= 0.f)
		{
			
			Player->Roll();
		}
		else
			Player->Backstep();
		
		return;
	}

	if (Player->GetState()->IsStateIdle())
	{
		Player->Jump();
		return;
	}

	if (Player->GetState()->IsStateParkour())
	{
		if (Input->GetAxisValue("MoveRight") > 0.5f)
		{
			CheckNull(Parkour);
			Parkour->ParkourRightJump();

			return;
		}
		
		if (Input->GetAxisValue("MoveRight") < -0.5f)
		{
			CheckNull(Parkour);
			Parkour->ParkourLeftJump();

			return;
		}

		Player->GetState()->SetStateIdle();

		float speed = Player->GetCharacterMovement()->JumpZVelocity;
		Player->GetCharacterMovement()->AddImpulse(FVector(0, 0,
			speed*Parkour->GetJumpImpulseRate()), true);
	}
}

void ACPlayerController::OnMoveForward(float axis)
{
	CheckFalse(Player->GetState()->IsCanMove());

	if (Player->GetState()->IsStateParkour())
	{
		if (axis > 0.5f)
		{
			CheckNull(Parkour);
			Parkour->ParkourClimbUp();
		}

		return;
	}

	FVector directionVector = Player->GetControlRotation().Vector();

	Player->AddMovementInput(directionVector * axis);
}

void ACPlayerController::OnMoveRight(float axis)
{
	CheckFalse(Player->GetState()->IsCanMove());

	if (Player->GetState()->IsStateParkour())
	{
		CheckNull(Parkour);

		if (axis > 0.5f)
			Parkour->ParkourRight(GetWorld()->GetDeltaSeconds());
		if (axis < -0.5f)
			Parkour->ParkourLeft(GetWorld()->GetDeltaSeconds());

		return;
	}

	FVector directionVector = UKismetMathLibrary::GetRightVector(Player->GetControlRotation());

	Player->AddMovementInput(directionVector * axis);
}

void ACPlayerController::OnHorizontalLook(float axis)
{
	Player->AddControllerYawInput(axis * GetWorld()->DeltaTimeSeconds * 50.f);
}

void ACPlayerController::OnVerticalLook(float axis)
{
	Player->AddControllerPitchInput(-axis * GetWorld()->DeltaTimeSeconds * 50.f);
}

void ACPlayerController::OnSword()
{
	Player->OnWeapon(0);
	
}

void ACPlayerController::OnDual()
{
	Player->OnWeapon(1);

}

void ACPlayerController::OnSpear()
{
	Player->OnWeapon(2);

}

void ACPlayerController::OnMagic()
{
	Player->OnWeapon(3);

}

void ACPlayerController::OnAttack()
{
	Player->Attack();
}

void ACPlayerController::OnSkill(int32 InIndex)
{
	Player->Skill(InIndex);
}

void ACPlayerController::OnTargetting()
{
	Player->Targetting();
}

void ACPlayerController::OnTargetChangeLeft()
{
	Player->TargetChange(false);
}

void ACPlayerController::OnTargetChangeRight()
{
	Player->TargetChange(true);
}

void ACPlayerController::OnExit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit, false);
}