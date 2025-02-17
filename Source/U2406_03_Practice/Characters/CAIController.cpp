

#include "Characters/CAIController.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CWeaponComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
ACAIController::ACAIController()
{
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this,&Perception, "Perception");

	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");

	Sight->SightRadius = 600;//시야반경
	Sight->LoseSightRadius = 800;
	Sight->PeripheralVisionAngleDegrees = 45.f;//좌우 90도
	Sight->SetMaxAge(2);//범위에서 벗어날경우 2초 후 감지멈춤

	Sight->DetectionByAffiliation.bDetectEnemies = true;//적에대한 감지true
	Sight->DetectionByAffiliation.bDetectNeutrals = false;	//중립에 대한 감지fasle
	Sight->DetectionByAffiliation.bDetectFriendlies = false;//아군에 대한 감지false

	//감각설정
	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
	
	
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	//감지가 될떄마다 호출되는 델리게이트
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdate);
	
}


void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	Enemy = Cast<ACEnemy_AI>(InPawn);
	SetGenericTeamId(Enemy->GetTeamID());

	CheckNull(Enemy->GetBehaviorTree());

	UBlackboardComponent* blackboard = Blackboard.Get();
	UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset, blackboard);
	
	Behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(Enemy);
	Behavior->SetBlackboard(Blackboard);	//현재 사용하는 블랙보드를 Behavior컴포넌트도 사용하게함.

	RunBehaviorTree(Enemy->GetBehaviorTree());


}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();

	
}

void ACAIController::OnPerceptionUpdate(const TArray<AActor*>& UpdatedActors)
{
	//사용할감각, 리턴받을배열
	CheckFalse(bCanUpdateTree);
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);

	/*for (AActor* actor : actors)
		CLog::Print(actor->GetName());*/

	if (actors.Num() > 0)
	{
		Blackboard->SetValueAsObject("Target", actors[0]);
		return;
	}

	Blackboard->SetValueAsObject("Target", nullptr);

}

void ACAIController::SetSightRadius(float InSightRadius)
{
	CheckNull(Sight);

	Sight->SightRadius=InSightRadius;
}