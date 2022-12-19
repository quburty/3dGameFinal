#include "CPatrolComponent.h"
#include "Global.h"
#include "Components/SplineComponent.h"
#include "Etc/CPatrolPath.h"

UCPatrolComponent::UCPatrolComponent()
{
	
}

bool UCPatrolComponent::GetMoveTo(FVector& OutLocation, float& OutAcceptanceRadius)
{
	OutLocation = FVector(0);
	OutAcceptanceRadius = 0;

	CheckNullResult(Path, false);

	OutLocation = Path->GetSpline()->GetTransformAtSplinePoint(Index, ESplineCoordinateSpace::World).GetLocation();
	OutAcceptanceRadius = AcceptanceRadius;
	return true;
}

void UCPatrolComponent::UpdateNextIndex()
{
	CheckNull(Path);

	int32 size = Path->GetSpline()->GetNumberOfSplinePoints();

	if (bReverse)
	{
		if (Index > 0)
		{
			Index--;
			return;
		}

		if (Path->IsLoop())
		{
			Index = size - 1;
			return;
		}
		else
		{
			bReverse = true;
			Index++;
			return;
		}
	}
	else
	{
		if (Index < size - 1)
		{
			Index++;
			return;
		}

		if (Path->IsLoop())
		{
			Index = 0;
			return;
		}
		else
		{
			bReverse = true;
			Index--;
			return;
		}
	}
}

void UCPatrolComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

