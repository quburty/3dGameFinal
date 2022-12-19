#include "CAnimNotify_EndHook.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/SkeletalMeshComponent.h"

FString UCAnimNotify_EndHook::GetNotifyName_Implementation() const
{
	return "EndHook";
}

void UCAnimNotify_EndHook::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->Hook_End();
}
