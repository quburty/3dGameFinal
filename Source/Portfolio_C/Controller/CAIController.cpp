#include "CAIController.h"
#include "Global.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Characters/CPlayer.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CharacterInterface.h"
#include "Components/CStateComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

ACAIController::ACAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	Perception = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600.0f;
	Sight->LoseSightRadius = 800.0f;
	Sight->PeripheralVisionAngleDegrees = 90.0f;
	Sight->SetMaxAge(2.0f);

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

float ACAIController::GetSightRadius()
{
	return Sight->SightRadius;
}

void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckFalse(bAlwaysPerceptPlayer);
	CheckNull(Blackboard);
	if (Blackboard->GetValueAsObject("Player") == NULL)
		Blackboard->SetValueAsObject("Player", UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	EnemyCharacter = Cast<ICharacter>(InPawn);
	SetGenericTeamId(EnemyCharacter->GetTeamID());

	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
	bAlwaysPerceptPlayer = EnemyCharacter->IsAlwaysPerceptPlayer();

	CheckNull(EnemyCharacter->GetBehaviorTree());

	UseBlackboard(EnemyCharacter->GetBehaviorTree()->BlackboardAsset, Blackboard);

	RunBehaviorTree(EnemyCharacter->GetBehaviorTree());

}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();

	Perception->OnPerceptionUpdated.Clear();
}

FPathFollowingRequestResult ACAIController::MoveTo(const FAIMoveRequest & MoveRequest, FNavPathSharedPtr * OutPath)
{
	FPathFollowingRequestResult fail;
	fail.Code = EPathFollowingRequestResult::Failed;

	CheckNullResult(EnemyCharacter, fail);
	CheckNullResult(EnemyCharacter->GetState(), fail);
	CheckFalseResult(EnemyCharacter->GetState()->IsCanMove(), fail);

	return Super::MoveTo(MoveRequest, OutPath);
}

void ACAIController::SetBehaviorTree(UBehaviorTree * InBehaviorTree)
{
	if(!!BrainComponent)
		BrainComponent->StopLogic("");
	//BrainComponent->UnregisterComponent();

	CheckNull(InBehaviorTree);

	UseBlackboard(InBehaviorTree->BlackboardAsset, Blackboard);
	RunBehaviorTree(InBehaviorTree);
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	CheckTrue(bAlwaysPerceptPlayer);

	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(NULL, actors);

	ACPlayer* player = NULL;
	for (AActor* actor : actors)
	{
		player = Cast<ACPlayer>(actor);

		if (!!player)
			break;
	}

	CheckNull(Blackboard);

	if (player == NULL)
	{
		GetWorldTimerManager().SetTimer
		(
			SetPlayerValueNullTimer, this,
			&ACAIController::SetPlayerValueNull,
			PerceptionRemainTime, false
		);
	}
	else
	{
		if (GetWorldTimerManager().IsTimerActive(SetPlayerValueNullTimer))
			GetWorldTimerManager().ClearTimer(SetPlayerValueNullTimer);

		Blackboard->SetValueAsObject("Player", player);
	}
}

void ACAIController::SetPlayerValue(class ACPlayer* InPlayer)
{
	CheckNull(Blackboard);
	CheckTrue(bAlwaysPerceptPlayer);

	//if already player exist
	if (!!Blackboard->GetValueAsObject("Player"))
		return;

	Blackboard->SetValueAsObject("Player", InPlayer);

	GetWorldTimerManager().SetTimer
	(
		SetPlayerValueNullTimer, this,
		&ACAIController::SetPlayerValueNull,
		PerceptionRemainTime, false
	);
}

void ACAIController::SetPlayerValueNull()
{
	CheckTrue(bAlwaysPerceptPlayer);
	Blackboard->SetValueAsObject("Player", NULL);
}