#include "CAnimNotify_EndSheath.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CCombatComponent.h"

FString UCAnimNotify_EndSheath::GetNotifyName_Implementation() const
{
	return "EndSheath";
}

void UCAnimNotify_EndSheath::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	ACharacter* owner = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	UCCombatComponent* combat = Cast<UCCombatComponent>(owner->GetComponentByClass(UCCombatComponent::StaticClass()));
	if (!!combat)
		combat->EndSheath();

	UCStateComponent* state = Cast<UCStateComponent>(owner->GetComponentByClass(UCStateComponent::StaticClass()));
	CheckNull(state);

	state->SetStateIdle();
}
