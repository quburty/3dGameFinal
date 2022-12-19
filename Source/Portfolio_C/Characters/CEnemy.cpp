#include "CEnemy.h"
#include "Global.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CStateComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Widgets/CWProgressBar.h"
#include "Widgets/CWMain.h"
#include "Widgets/CHUD.h"

ACEnemy::ACEnemy()
{
 	PrimaryActorTick.bCanEverTick = true;

	Status = CreateDefaultSubobject<UCStatusComponent>("Status");
	Montage = CreateDefaultSubobject<UCMontagesComponent>("Montage");
	State = CreateDefaultSubobject<UCStateComponent>("State");
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>("HealthBarWidget");

	if (!!HealthBarWidget)
	{
		HealthBarWidget->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		HealthBarWidget->SetRelativeLocation(FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2 + 30.f));
		HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
		HealthBarWidget->SetDrawSize(FVector2D(200, 50));

		TSubclassOf<UUserWidget> widgetClass = UUserWidget::StaticClass();
		CHelpers::GetClass(&widgetClass, "WidgetBlueprint'/Game/Widget/CWB_HealthBar.CWB_HealthBar_C'");

		if (!!widgetClass)
			HealthBarWidget->SetWidgetClass(widgetClass);
		
	}
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (!!HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);

		if (HealthBarType == EHealthBarType::Boss)
		{
			////AHUD* hud = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD();
			////HealthBarWidget->AttachToComponent(hud->GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
			//
			//TSubclassOf<UUserWidget> widgetClass = UUserWidget::StaticClass();
			//CHelpers::GetClass(&widgetClass, "WidgetBlueprint'/Game/Widget/CWB_HealthBar.CWB_HealthBar_C'");
			//
			//if (!!widgetClass)//crash
			//{
			//	UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), widgetClass);
			//	widget->AddToViewport();
			//}

		}
	}
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACEnemy::Hitted_Implementation()
{
	if (!!HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);

		FTimerDelegate setInvisible;
		setInvisible.BindUFunction(HealthBarWidget, "SetVisibility", false);
		
		GetWorldTimerManager().ClearTimer(HealthBarHandle);
		GetWorldTimerManager().SetTimer(HealthBarHandle, setInvisible, 3.f, false);

		UCWProgressBar* widget = Cast<UCWProgressBar>(HealthBarWidget->GetUserWidgetObject());
		if (!!widget)
			widget->UpdateProgressBar(Status->GetHealth(), Status->GetMaxHealth());
	}

	CheckNull(Montage);
	CheckNull(State);

	switch (Status->GetSuperArmor())
	{
	case ESuperArmor::Action:
		if (State->IsStateIdle())break;
		else return;
	case ESuperArmor::HealthRate:
		if (Status->IsSuperArmorDamagedEnough())break;
		else return;
	default:break;
	}

	if (!!HittedEffect)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HittedEffect, GetActorLocation());

	const FAssetMontageData* data = Montage->GetMontageData_Hit();
	CheckNull(data);
	CheckNull(data->AnimMontage);

	PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
	data->bCanMove ? State->SetMove() : State->SetStop();

	State->SetStateHitted();
}

float ACEnemy::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CheckTrueResult(Status->IsDead(), 0.0f);

	Status->ReduceHealth(DamageAmount);

	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ACHUD* hud = Cast<ACHUD>(controller->GetHUD());
	if (!!hud)
	{
		FVector2D screenPos;
		controller->ProjectWorldLocationToScreen(this->GetActorLocation(), screenPos);
		hud->GetMainWidget()->PrintDamage(screenPos, DamageAmount);
	}

	Hitted();

	if (Status->IsDead())
	{


		Dead();
	}

	return DamageAmount;
}

void ACEnemy::Dead()
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
