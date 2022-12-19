#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CTargetComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_C_API UCTargetComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		class UParticleSystem* Particle;

	UPROPERTY(EditAnywhere)
		FVector AttachOffset;

	UPROPERTY(EditAnywhere)
		float TraceRadius = 1000.0f;

	UPROPERTY(EditAnywhere)
		bool bDrawDebug;

	UPROPERTY(EditAnywhere)
		TEnumAsByte<ETraceTypeQuery> TraceType;

	UPROPERTY(EditAnywhere)
		float InteropSpeed = 10.f;

public:
	FORCEINLINE bool IsTargeting() { return !!Target ? true : false; }

public:	
	UCTargetComponent();

protected:
	virtual void BeginPlay() override;

public:
	void ToggleTarget();
	void ChangeTarget(bool InRight);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void StartTargeting();
	void EndTargeting();

	void SetTraceTargets();
	void SetTarget();

	void ChangeCursor(class ACharacter* InCharacter);

private:
	class ACharacter* OwnerCharacter;

	TArray<class ACharacter*> TraceTargets;
	class ACharacter* Target;

	class UParticleSystemComponent* Attached;
};
