#include "Combats/Weapons/CWeaponMagic.h"
#include "Global.h"
#include "CFire.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

void ACWeaponMagic::BeginPlay()
{
	Super::BeginPlay();
}

void ACWeaponMagic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Angle += SpinSpeed * DeltaTime;
	UKismetMathLibrary::FMod(Angle, UKismetMathLibrary::GetPI() * 2, Angle);

	for (int32 i = 0; i < Fires.Num(); i++)
	{
		FVector position = Owner->GetActorLocation();
		position.Y += FMath::Cos(Angle + (UKismetMathLibrary::GetPI() * 2 / Fires.Num())*i) * SpinRadius;
		position.X += FMath::Sin(Angle + (UKismetMathLibrary::GetPI() * 2 / Fires.Num())*i) * SpinRadius;

		Fires[i]->SetActorLocation(position);
	}

	Time += DeltaTime;
	if (Time > HitTimeInterval)
	{
		UKismetMathLibrary::FMod(Time, HitTimeInterval, Time);

		for (AActor* target : TargetActors)
		{
			target->TakeDamage(1.0f, FDamageEvent(), nullptr, this);
		}
	}
	
}

void ACWeaponMagic::GrabWeapon_Implementation()
{
	Super::GrabWeapon_Implementation();

	CheckNull(Owner);
	for (uint32 i = 0; i < FireCount; i++)
	{
		ACFire* fire = GetWorld()->SpawnActor<ACFire>(FireClass, FTransform());

		//ACFire* fire = GetWorld()->SpawnActorDeferred<ACFire>(FireClass, FTransform(), Owner);
		//fire->FinishSpawning(FTransform());

		if (!!fire)
		{
			Fires.Add(fire);
			Colliders.Add(fire->GetShapeComponent());
			Colliders.Last()->OnComponentBeginOverlap.Clear();
			Colliders.Last()->OnComponentBeginOverlap.AddDynamic(this, &ACWeaponMagic::BeginOverlap);
			Colliders.Last()->OnComponentEndOverlap.Clear();
			Colliders.Last()->OnComponentEndOverlap.AddDynamic(this, &ACWeaponMagic::EndOverlap);
		}
	}
}

void ACWeaponMagic::UngrabWeapon_Implementation()
{
	Super::UngrabWeapon_Implementation();

	for (ACFire* fire : Fires)
	{
		if (!!fire)
		{
			fire->Destroy();
		}
	}
	Colliders.Empty();
	Fires.Empty();
	TargetActors.Empty();
}

void ACWeaponMagic::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	CheckNull(Cast<ACharacter>(OtherActor));
	if (HittedCharacters.Find(Cast<ACharacter>(OtherActor))) //적이 타격되었을 경우
	{
		TargetActors.Add(OtherActor);
	}
}

void ACWeaponMagic::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TargetActors.Remove(OtherActor);

	CheckNull(Cast<ACharacter>(OtherActor));
	HittedCharacters.Remove(Cast<ACharacter>(OtherActor));
}

void ACWeaponMagic::HitCharacter(AActor* DamagedActor, AController* EventInstigator, AActor* DamageCauser)
{
	Super::HitCharacter(DamagedActor, EventInstigator, DamageCauser);

}
