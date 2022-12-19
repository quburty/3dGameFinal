#include "Combats/Weapons/CWeaponMelee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/ShapeComponent.h"
#include "Components/CapsuleComponent.h"

void ACWeaponMelee::BeginPlay()
{
	Super::BeginPlay();

	if (bUseSkeletalWeapon)
	{
		UCapsuleComponent* capsule = NewObject<UCapsuleComponent>(this, "Capsule");
		capsule->RegisterComponent();
		capsule->AttachToComponent(RootComponent,
			FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), "Capsule");

		FVector start = Owner->GetMesh()->GetSocketLocation(SocketBegin);
		FVector end = Owner->GetMesh()->GetSocketLocation(SocketEnd);

		//capsule->SetRelativeLocation((start + end)*0.5f);
		capsule->SetRelativeRotation(FRotator(90, 0, 0));
		capsule->SetCapsuleHalfHeight(FVector::Distance(start, end)*0.5f);
		capsule->SetCapsuleRadius(SkeletalWeaponRadius);

		AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), SocketBegin);
		SetActorRelativeLocation((end - start)*0.5f);

		FRotator rot = (end - start).ToOrientationRotator();
		SetActorRelativeRotation(rot);

		Colliders.Add(capsule);
		Colliders.Last()->OnComponentBeginOverlap.AddDynamic(this, &ACWeaponMelee::BeginOverlap);

	}
	else
	{
		//TArray<UActorComponent*> components = GetComponentsByClass(UShapeComponent::StaticClass());
		TArray<UShapeComponent*> components;
		GetComponents<UShapeComponent>(components);
		for (UShapeComponent* component : components)
		{
			Colliders.Add(component);
			Colliders.Last()->OnComponentBeginOverlap.AddDynamic(this, &ACWeaponMelee::BeginOverlap);
		}
	}

	OffCollision();
}

