#include "CEnemy_AI.h"
#include "Global.h"
#include "Perception/AIPerceptionComponent.h"
#include "Controller/CAIController.h"
#include "CPlayer.h"

ACEnemy_AI::ACEnemy_AI()
{
	AIControllerClass = ACAIController::StaticClass();
}

float ACEnemy_AI::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float value = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CheckNullResult(EventInstigator, value);
	CheckNullResult(EventInstigator->GetPawn(), value);

	ACPlayer* player = Cast<ACPlayer>(EventInstigator->GetPawn());
	
	if (!!player)
	{
		ACAIController* controller = GetController<ACAIController>();
		controller->SetPlayerValue(player);
	}

	return value;
}