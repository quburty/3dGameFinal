#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CHUD.generated.h"

UCLASS()
class PORTFOLIO_C_API ACHUD : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		TSubclassOf<class UCWMain> WidgetClass;

public:
	FORCEINLINE class UCWMain* GetMainWidget() { return MainWidget; }
	
protected:
	void BeginPlay() override;

public:
	virtual void DrawHUD() override;

private:
	class UCWMain* MainWidget;

	class UCStatusComponent* Status;
	class UCCombatComponent* Combat;

};
