#include "Widgets/CWDamageFont.h"
#include "Global.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"

UCWDamageFont::UCWDamageFont(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
	//UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());

	//Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), "DamageText");
	//RootWidget->AddChild(Text);

	Text = NewObject<UTextBlock>();
	Text->AddToRoot();
}

void UCWDamageFont::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UCWDamageFont::NativeConstruct()
{
	Super::NativeConstruct();

	Position = this->RenderTransform.Translation;
	float angle = UKismetMathLibrary::RandomFloatInRange(UKismetMathLibrary::GetPI()* 0.4, UKismetMathLibrary::GetPI()*0.6);
	Direction.X = FMath::Cos(angle) * InitialSpeed;
	Direction.Y = -FMath::Sin(angle) * InitialSpeed;
}

void UCWDamageFont::SetValue(const float& InValue)
{
	Text->SetText(FText::FromString(FString::FromInt((int32)InValue)));
}

void UCWDamageFont::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	Direction.Y += InDeltaTime * Gravity;

	this->SetRenderTranslation(this->RenderTransform.Translation + (Direction*InDeltaTime));
}