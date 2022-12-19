#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CFire.generated.h"

UCLASS()
class PORTFOLIO_C_API ACFire : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
		class USphereComponent* Sphere;
	
public:
	class UShapeComponent* GetShapeComponent();

public:	
	ACFire();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
