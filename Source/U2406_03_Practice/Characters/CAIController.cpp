

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

	Sight->SightRadius = 600;//�þ߹ݰ�
	Sight->LoseSightRadius = 800;
	Sight->PeripheralVisionAngleDegrees = 45.f;//�¿� 90��
	Sight->SetMaxAge(2);//�������� ������ 2�� �� ��������

	Sight->DetectionByAffiliation.bDetectEnemies = true;//�������� ����true
	Sight->DetectionByAffiliation.bDetectNeutrals = false;	//�߸��� ���� ����fasle
	Sight->DetectionByAffiliation.bDetectFriendlies = false;//�Ʊ��� ���� ����false

	//��������
	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
	
	
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	//������ �ɋ����� ȣ��Ǵ� ��������Ʈ
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
	Behavior->SetBlackboard(Blackboard);	//���� ����ϴ� �����带 Behavior������Ʈ�� ����ϰ���.

	RunBehaviorTree(Enemy->GetBehaviorTree());


}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();

	
}

void ACAIController::OnPerceptionUpdate(const TArray<AActor*>& UpdatedActors)
{
	//����Ұ���, ���Ϲ����迭
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