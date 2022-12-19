#include "CAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"

UCAction::UCAction()
	:Super()
{
}

void UCAction::PostInitProperties()
{
	Super::PostInitProperties();

	if (!!Cast<ACharacter>(GetOuter()))
	{
		Owner = Cast<ACharacter>(GetOuter());
		State = Cast<UCStateComponent>(Owner->GetComponentByClass(UCStateComponent::StaticClass()));
	}
	
}

void UCAction::Tick(float DeltaTime)
{
	//if (UseCooldown && Cooldown > 0.0f)
	//	Cooldown -= DeltaTime;
}

bool UCAction::Action()
{
	CheckFalseResult(Index < AttackDatas.Num(), false);

	Power = AttackDatas[Index].Power;
	return true;
}

void UCAction::HitCharacter(AActor* DamagedActor, AController* EventInstigator,AActor* DamageCauser)
{
	if (AttackDatas.Num() <= Index)
	{
		Log_Error("Combo Index is out of range.");
		return;
	}

	const FAttackData& data = AttackDatas[Index];

	if (FMath::IsNearlyZero(data.HitStop) == false)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), data.HitStopSpeed);
		UKismetSystemLibrary::K2_SetTimer(this, "OnRestorDilation", data.HitStop * data.HitStopSpeed, false);
	}

	if (!!data.HitEffect)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), data.HitEffect, DamagedActor->GetActorLocation());

	if (FMath::IsNearlyZero(Power))
		Log_Warning("Power is 0. TakeDamage() may not be called.");

	DamagedActor->TakeDamage(Power, FDamageEvent(), EventInstigator, DamageCauser);
}

void UCAction::OnRestorDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
}
