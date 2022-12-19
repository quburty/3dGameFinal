#include "CAnimNotify_EndParkour.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CParkourComponent.h"

FString UCAnimNotify_EndParkour::GetNotifyName_Implementation() const
{
	return "EndParkour";
}

void UCAnimNotify_EndParkour::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	ACharacter* owner = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(owner);
	CheckNull(parkour);

	parkour->EndParkourAction();
}
