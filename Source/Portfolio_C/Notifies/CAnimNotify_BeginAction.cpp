#include "CAnimNotify_BeginAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CCombatComponent.h"

FString UCAnimNotify_BeginAction::GetNotifyName_Implementation() const
{
	return "BeginAction";
}

void UCAnimNotify_BeginAction::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	ACharacter* owner = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	UCCombatComponent* combat = Cast<UCCombatComponent>(owner->GetComponentByClass(UCCombatComponent::StaticClass()));
	CheckNull(combat);

	combat->BeginAction();
}
