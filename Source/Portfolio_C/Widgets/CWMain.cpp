#include "CWMain.h"
#include "Global.h"
#include "CWDamageFont.h"

void UCWMain::RegisterHealthBar(class UCWProgressBar* InWidget)
{
	HealthBar = InWidget;
}

void UCWMain::AddSkillIcons(class UCWSkillIcon * InWidget)
{
	CheckNull(InWidget);
	SkillIcons.Add(InWidget);
}

void UCWMain::PrintDamage(const FVector2D& Pos, float Damage)
{
	//CLog::Print(Damage);
	UCWDamageFont* damageFont = CreateWidget<UCWDamageFont>(this, DamageFont);
	if (!!damageFont)
	{
		damageFont->AddToViewport();
		damageFont->SetPositionInViewport(Pos);
		damageFont->SetValue(Damage);

		FTimerHandle timerHandle;
		if (!!GetWorld())
		{
			FTimerDelegate remove;
			remove.BindUFunction(damageFont, "RemoveFromParent");

			GetWorld()->GetTimerManager().SetTimer(timerHandle, remove, 1.0f, false);
		}
	}
}