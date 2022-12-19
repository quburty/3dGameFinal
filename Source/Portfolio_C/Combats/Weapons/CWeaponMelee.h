#pragma once

#include "CoreMinimal.h"
#include "Combats/Weapons/CWeapon.h"
#include "CWeaponMelee.generated.h"

UCLASS()
class PORTFOLIO_C_API ACWeaponMelee : public ACWeapon
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		bool bUseSkeletalWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		FName SocketBegin = "None";

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		FName SocketEnd = "None";

	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		float SkeletalWeaponRadius = 5.f;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override {}

};
