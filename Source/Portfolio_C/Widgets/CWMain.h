#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWMain.generated.h"

UCLASS()
class PORTFOLIO_C_API UCWMain : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE class UCWProgressBar* GetHealthBar() { return HealthBar; }
	FORCEINLINE TArray<class UCWSkillIcon *> GetSkillIcons() { return SkillIcons; }

protected:
	UFUNCTION(BlueprintCallable)
		void RegisterHealthBar(class UCWProgressBar* InWidget);

	UFUNCTION(BlueprintCallable)
		void AddSkillIcons(class UCWSkillIcon * InWidget);

public:
	void PrintDamage(const FVector2D& Pos, float Damage);

private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCWDamageFont> DamageFont;

	class UCWProgressBar* HealthBar;
	TArray<class UCWSkillIcon*> SkillIcons;

};
