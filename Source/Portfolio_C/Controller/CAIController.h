#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

UCLASS()
class PORTFOLIO_C_API ACAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		float BehaivorRange = 150.0f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = 0))
		float PerceptionRemainTime = 2.0f;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UAIPerceptionComponent* Perception;

public:
	FORCEINLINE float GetBehaviorRange() { return BehaivorRange; }
	
public:
	ACAIController();
	float GetSightRadius();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	virtual FPathFollowingRequestResult MoveTo(const FAIMoveRequest& MoveRequest, FNavPathSharedPtr* OutPath = nullptr) override;

public:
	UFUNCTION(BlueprintCallable)
		void SetBehaviorTree(class UBehaviorTree* InBehaviorTree);

private:
	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

public:
	UFUNCTION()
		void SetPlayerValue(class ACPlayer* InPlayer);

private:
	UFUNCTION()
		void SetPlayerValueNull();

private:
	class ICharacterInterface* EnemyCharacter;
	class UAISenseConfig_Sight* Sight;

	FTimerHandle SetPlayerValueNullTimer;
	bool bAlwaysPerceptPlayer;
};
