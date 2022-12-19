#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_SpawnThrow.generated.h"

UCLASS()
class PORTFOLIO_C_API UCAnimNotify_SpawnThrow : public UAnimNotify
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACThrow> ThrowClass;

	UPROPERTY(EditAnywhere)
		float ProjectileSpeed = 500.f;

public:
	FString GetNotifyName_Implementation() const override;

	virtual void Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation) override;

};
