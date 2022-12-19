#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CWDamageFont.generated.h"

UCLASS()
class PORTFOLIO_C_API UCWDamageFont : public UUserWidget
{
	GENERATED_BODY()

public:
	UCWDamageFont(const FObjectInitializer &ObjectInitializer);

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	void SetValue(const float& InValue);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	//UPROPERTY(meta = (AllowPrivateAccess = "true", BindWidget))
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true", BindWidget))
		class UTextBlock* Text;

	UPROPERTY(EditDefaultsOnly)
		float InitialSpeed = 50.f;

	UPROPERTY(EditDefaultsOnly)
		float Gravity = 0.2f;

private:
	FVector2D Position;
	FVector2D Direction;

};
