#include "CAnimNotify_EndEquip.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CCombatComponent.h"

FString UCAnimNotify_EndEquip::GetNotifyName_Implementation() const
{
	return "EndEquip";
}

void UCAnimNotify_EndEquip::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	ACharacter* owner = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	UCCombatComponent* combat = Cast<UCCombatComponent>(owner->GetComponentByClass(UCCombatComponent::StaticClass()));
	if (!!combat)
		combat->EndEquip();

	UCStateComponent* state = Cast<UCStateComponent>(owner->GetComponentByClass(UCStateComponent::StaticClass()));
	CheckNull(state);

	state->SetStateIdle();
}
