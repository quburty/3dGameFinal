#include "CMontagesComponent.h"
#include "Global.h"
#include "Engine/DataTable.h"

// Sets default values for this component's properties
UCMontagesComponent::UCMontagesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

const FAssetMontageData * UCMontagesComponent::GetMontageData_Backstep()
{
	CheckNullResult(MontagesData, NULL);
	return MontagesData->GetBackstepMontageData();
}

const FAssetMontageData * UCMontagesComponent::GetMontageData_Roll()
{
	CheckNullResult(MontagesData, NULL);
	return MontagesData->GetRollMontageData();
}

const FAssetMontageData * UCMontagesComponent::GetMontageData_Hit()
{
	CheckNullResult(MontagesData, NULL);
	return MontagesData->GetHitMontageData();
}

const FAssetMontageData * UCMontagesComponent::GetMontageData_Die()
{
	CheckNullResult(MontagesData, NULL);
	return MontagesData->GetDieMontageData();
}

// Called when the game starts
void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UCMontagesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

