#include "CAnimNotify_GrabWeapon.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CCombatComponent.h"

FString UCAnimNotify_GrabWeapon::GetNotifyName_Implementation() const
{
	return "GrabWeapon";
}

void UCAnimNotify_GrabWeapon::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	ACharacter* owner = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	UCCombatComponent* combat = Cast<UCCombatComponent>(owner->GetComponentByClass(UCCombatComponent::StaticClass()));
	CheckNull(combat);

	combat->GrabWeapon();
}
