#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CIKComponent.generated.h"

USTRUCT(BlueprintType)
struct FIKData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector PelvisDistance; //Z축만 사용

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector LeftDistance; //X축만 사용

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FVector RightDistance; //X축만 사용

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator LeftRotation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FRotator RightRotation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PORTFOLIO_C_API UCIKComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UserSettings")
		bool bDrawDebug;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "UserSettings")
		float TraceDistance = 55.0f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "UserSettings")
		float OffsetDistance = 5.0f; //발과 땅 사이에 간격

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "UserSettings")
		float InterpSpeed = 22.0f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "UserSettings")
		FName LeftSocket = "Foot_L";

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "UserSettings")
		FName RightSocket = "Foot_R";

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "UserSettings")
		float PelvisInteropSpeed = 17.0f;

public:
	UCIKComponent();

public:
	FORCEINLINE const FIKData& GetFeetData() { return FeetData; }

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void Trace(FName InSocket, float& OutDistance, FRotator& OutRotation);

private:
	class ACharacter* OwnerCharacter;

	FIKData FeetData;

	float ScaledHalfHeight;
};
