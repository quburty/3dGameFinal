#include "CEnvQueryContext_Player.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UCEnvQueryContext_Player::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	//ACEnemy_AI* ai = Cast<ACEnemy_AI>(QueryInstance.Owner.Get());

	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, player);
}