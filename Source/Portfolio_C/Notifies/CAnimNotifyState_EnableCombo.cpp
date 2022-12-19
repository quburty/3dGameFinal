#include "CAnimNotifyState_EnableCombo.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CCombatComponent.h"

FString UCAnimNotifyState_EnableCombo::GetNotifyName_Implementation() const
{
	return "EnableCombo";
}

void UCAnimNotifyState_EnableCombo::NotifyBegin(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, float TotalDuration)
{
	ACharacter* owner = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	UCCombatComponent* combat = Cast<UCCombatComponent>(owner->GetComponentByClass(UCCombatComponent::StaticClass()));
	CheckNull(combat);

	combat->OnEnableCombo();
}

void UCAnimNotifyState_EnableCombo::NotifyEnd(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	ACharacter* owner = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	UCCombatComponent* combat = Cast<UCCombatComponent>(owner->GetComponentByClass(UCCombatComponent::StaticClass()));
	CheckNull(combat);

	combat->OffEnableCombo();
}