#include "CAnimNotify_SpawnThrow.h"
#include "Global.h"
#include "Etc/CThrow.h"
#include "GameFramework/Character.h"

FString UCAnimNotify_SpawnThrow::GetNotifyName_Implementation() const
{
	return "SpawnThrow";
}

void UCAnimNotify_SpawnThrow::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	ACharacter* owner = Cast<ACharacter>(MeshComp->GetOwner());

	CheckNull(owner);
	CheckNull(ThrowClass);

	UWorld* world = MeshComp->GetWorld();
	CheckNull(world);

	FTransform spawnTransform(FRotator(), owner->GetActorLocation());

	ACThrow* throwActor = world->SpawnActorDeferred<ACThrow>(ThrowClass, spawnTransform, owner);
	CheckNull(throwActor);
	throwActor->FinishSpawning(spawnTransform);

	FVector velocity = owner->GetActorRotation().Vector();
	velocity *= ProjectileSpeed;

	throwActor->SetProjectileVelocity(velocity);

}
