#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPatrolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_C_API UCPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditInstanceOnly, Category = "UserSettings")
		class ACPatrolPath* Path;

	UPROPERTY(EditAnywhere)
		bool bReverse;

	UPROPERTY(EditAnywhere)
		float AcceptanceRadius = 50.0f;

public:
	FORCEINLINE bool IsValid() { return Path != NULL; }

public:	
	UCPatrolComponent();

	bool GetMoveTo(FVector& OutLocation, float& OutAcceptanceRadius);
	void UpdateNextIndex();

protected:
	virtual void BeginPlay() override;

private:
	int32 Index = 0;
};
