#include "CAnimNotify_EndAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CCombatComponent.h"

FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
	return "EndAction";
}

void UCAnimNotify_EndAction::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	ACharacter* owner = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	UCCombatComponent* combat = Cast<UCCombatComponent>(owner->GetComponentByClass(UCCombatComponent::StaticClass()));
	if (!!combat)
		combat->EndAction();

	UCStateComponent* state = Cast<UCStateComponent>(owner->GetComponentByClass(UCStateComponent::StaticClass()));
	CheckNull(state);

	state->SetStateIdle();
	state->SetMove();
	
}
