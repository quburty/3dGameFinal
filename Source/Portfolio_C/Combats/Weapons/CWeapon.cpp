#include "CWeapon.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/ShapeComponent.h"
//#include "DestructibleComponent.h"

ACWeapon::ACWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	Owner = Cast<ACharacter>(GetOwner());
	CheckNull(Owner);

}

void ACWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACWeapon::GrabWeapon_Implementation()
{
	
}

void ACWeapon::UngrabWeapon_Implementation()
{

}

ACharacter* ACWeapon::GetOwnerCharacter()
{
	return Owner;
}

void ACWeapon::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	CheckTrue(OtherActor == Owner);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (character == NULL)
	{
		OtherActor->TakeDamage(1.0f, FDamageEvent(), Owner->GetController(), this);
		return;
	}

	if (!!HittedCharacters.Find(character))
		return;
	
	HittedCharacters.Add(character);
	
	if (OnWeaponHitDelegate.IsBound())
		OnWeaponHitDelegate.Broadcast(OtherActor, Owner->GetController(), this);
}

void ACWeapon::OnCollision()
{
	for (UShapeComponent* collider : Colliders)
	{
		collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ACWeapon::OffCollision()
{
	for (UShapeComponent* collider : Colliders)
	{
		collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	HittedCharacters.Empty();
}

void ACWeapon::HitCharacter(AActor* DamagedActor, AController* EventInstigator, AActor* DamageCauser)
{
}

void ACWeapon::AttachToSocket(FName SocketName)
{
	CheckNull(Owner);

	AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), SocketName);
}