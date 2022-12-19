#include "CAnimNotify_BeginHook.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/SkeletalMeshComponent.h"

FString UCAnimNotify_BeginHook::GetNotifyName_Implementation() const
{
	return "BeginHook";
}

void UCAnimNotify_BeginHook::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->Hook_Begin();
}
