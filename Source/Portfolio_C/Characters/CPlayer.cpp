#include "CPlayer.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CCombatComponent.h"
#include "Components/CTargetComponent.h"
#include "Components/CParkourComponent.h"
#include "Components/CIKComponent.h"
#include "Materials/MaterialInstance.h"
#include "Animation/AnimMontage.h"
#include "Etc/CHook.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring");
	SpringArm->SetupAttachment(GetMesh());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 0, 0));
	SpringArm->TargetArmLength = 280;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	Camera->SetRelativeLocation(FVector(-16, 80, 33));
	Camera->SetRelativeRotation(FRotator(-10, 0, 0));

	Montage = CreateDefaultSubobject<UCMontagesComponent>("Montages");
	Combat = CreateDefaultSubobject<UCCombatComponent>("Combat");
	State = CreateDefaultSubobject<UCStateComponent>("State");
	Status = CreateDefaultSubobject<UCStatusComponent>("Status");
	Parkour = CreateDefaultSubobject<UCParkourComponent>("Parkour");
	Target = CreateDefaultSubobject<UCTargetComponent>("Target");
	IK = CreateDefaultSubobject<UCIKComponent>("IK");
}

void ACPlayer::SetInvincibility()
{
	bInvincibility = !bInvincibility;
	if (bInvincibility)
		CLog::Print("Player is setted to invincible");
	else
		CLog::Print("Player is setted to non-invincible");

}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FGenericTeamId ACPlayer::GetGenericTeamId() const
{
	return FGenericTeamId(0);
}

void ACPlayer::Roll()
{
	CheckFalse(State->IsStateIdle());

	CheckNull(Montage->GetMontageData_Roll());
	const FAssetMontageData& data = *Montage->GetMontageData_Roll();

	CheckNull(data.AnimMontage);

	PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);
	State->SetStateAction();
}

void ACPlayer::Backstep()
{
	CheckFalse(State->IsStateIdle());

	CheckNull(Montage->GetMontageData_Roll());
	const FAssetMontageData& data = *Montage->GetMontageData_Backstep();

	CheckNull(data.AnimMontage);

	FRotator newRotator = GetActorRotation().Add(0, 180, 0);

	SetActorRotation(newRotator);

	PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);
	State->SetStateAction();
}

void ACPlayer::SetRotationLock()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void ACPlayer::SetRotationFree()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ACPlayer::OnWeapon(uint8 InWeaponIndex)
{
	CheckNull(Combat);
	
	Combat->OnWeapon(InWeaponIndex);
}

void ACPlayer::Attack()
{
	Combat->Attack();
}

void ACPlayer::Skill(int32 InIndex)
{
	Combat->Skill(InIndex);
}

void ACPlayer::Targetting()
{
	CheckNull(Target);
	Target->ToggleTarget();
}

void ACPlayer::TargetChange(bool InRight)
{
	CheckNull(Target);
	Target->ChangeTarget(InRight);
}

void ACPlayer::BeginPostProcess(UMaterialInstance* InPostProcess)
{
	CheckNull(InPostProcess);

	if (!!PostProcess)
		EndPostProcess();

	PostProcess = InPostProcess;
	Camera->PostProcessSettings.AddBlendable(InPostProcess, 1.0f);
}

void ACPlayer::EndPostProcess()
{
	CheckNull(PostProcess);
	Camera->PostProcessSettings.RemoveBlendable(PostProcess);
}

void ACPlayer::Hook()
{
	CheckNull(State);
	CheckFalse(State->IsStateIdle());

	FRotator rotator = GetActorRotation();
	rotator.Yaw = GetControlRotation().Yaw;
	SetActorRotation(rotator);

	if (!!HookMontage.AnimMontage)
	{
		PlayAnimMontage(HookMontage.AnimMontage, HookMontage.PlayRatio, HookMontage.StartSection);
		HookMontage.bCanMove ? State->SetMove() : State->SetStop();
	}
	State->SetStateAction();
}

void ACPlayer::Hook_Begin()
{
	ACHook* hook = GetWorld()->SpawnActorDeferred<ACHook>(HookActor, GetActorTransform(), this);
	hook->FinishSpawning(GetActorTransform());
	hook->SetProjectileDirection(GetControlRotation().Vector());
	//hook->SetOwner(this);
}

void ACPlayer::Hook_End()
{
	if(State->IsStateAction())
		State->SetStateIdle();
	State->SetMove();
}

float ACPlayer::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	CheckTrueResult(bInvincibility, 0.0f);
	CheckTrueResult(Status->IsDead(), 0.0f);

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Status->ReduceHealth(DamageAmount);

	Hitted();

	if (Status->IsDead())
		Dead();

	return DamageAmount;
}

void ACPlayer::Hitted()
{
	CheckNull(Montage);
	CheckNull(State);

	const FAssetMontageData* data = Montage->GetMontageData_Hit();
	CheckNull(data);
	CheckNull(data->AnimMontage);

	switch (Status->GetSuperArmor())
	{
	case ESuperArmor::None: break;
	case ESuperArmor::Action:
		if (State->IsStateIdle())
			break;
		else
			return;
	default:
		break;
	}

	PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
	data->bCanMove ? State->SetMove() : State->SetStop();

	State->SetStateHitted();
}

void ACPlayer::Dead()
{
	if (Montage == NULL)
	{
		Destroy();
		return;
	}

	const FAssetMontageData* data = Montage->GetMontageData_Die();

	if (data == NULL || data->AnimMontage == NULL)
	{
		Destroy();
		return;
	}

	float time = PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
	UKismetSystemLibrary::K2_SetTimer(this, "K2_DestroyActor", time * 0.7f, false);

}
