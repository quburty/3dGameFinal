#include "Widgets/CHUD.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "Components/CStatusComponent.h"
#include "Components/CCombatComponent.h"
#include "Widgets/CWMain.h"
#include "Widgets/CWProgressBar.h"
#include "Widgets/CWSkillIcon.h"

void ACHUD::BeginPlay()
{
	Super::BeginPlay();

	CheckNull(WidgetClass);
	MainWidget = NewObject<UCWMain>(this, WidgetClass);

	CheckNull(MainWidget);
	MainWidget->AddToViewport();

	PlayerOwner = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	APawn* pawn = PlayerOwner->GetPawn();

	CheckNull(pawn);
	Status = CHelpers::GetComponent<UCStatusComponent>(pawn);
	Combat = CHelpers::GetComponent<UCCombatComponent>(pawn);
}

void ACHUD::DrawHUD()
{
	Super::DrawHUD();

	CheckNull(MainWidget);

	UCWProgressBar* healthBar = MainWidget->GetHealthBar();
	healthBar->UpdateProgressBar(Status->GetHealth(), Status->GetMaxHealth());

	if (Combat->IsEquipped())
	{
		int32 count = FMath::Min<int32>(Combat->GetSkillsCount(), MainWidget->GetSkillIcons().Num());

		for (int32 i = 0; i < count; i++)
		{
			UCWSkillIcon* skillIcon = MainWidget->GetSkillIcons()[i];
			skillIcon->SetVisibility(ESlateVisibility::Visible);
			skillIcon->UpdateProgressBar(Combat->GetSkillsCooldown(i), Combat->GetSkillsMaxCooldown(i));
		}
	}
	else
	{
		for (UCWSkillIcon* widget : MainWidget->GetSkillIcons())
		{
			widget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}