#include "CAnimNotify_UngrabWeapon.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CCombatComponent.h"

FString UCAnimNotify_UngrabWeapon::GetNotifyName_Implementation() const
{
	return "UngrabWeapon";
}

void UCAnimNotify_UngrabWeapon::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	ACharacter* owner = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	UCCombatComponent* combat = Cast<UCCombatComponent>(owner->GetComponentByClass(UCCombatComponent::StaticClass()));
	CheckNull(combat);

	combat->UngrabWeapon();
}
