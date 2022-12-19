// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_EndSheath.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_C_API UCAnimNotify_EndSheath : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	FString GetNotifyName_Implementation() const override;

	virtual void Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation) override;

};