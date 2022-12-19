// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "DataAssets/DataAsset_Montages.h"
#include "CMontagesComponent.generated.h"

UENUM()
enum class EMontageType : int32
{
	Unknown, Hit, Die, Roll, Backstep, Max
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_C_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCMontagesComponent();

public:
	const FAssetMontageData* GetMontageData_Backstep();
	const FAssetMontageData* GetMontageData_Roll();
	const FAssetMontageData* GetMontageData_Hit();
	const FAssetMontageData* GetMontageData_Die();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		const class UDataAsset_Montages* MontagesData;
		
};
