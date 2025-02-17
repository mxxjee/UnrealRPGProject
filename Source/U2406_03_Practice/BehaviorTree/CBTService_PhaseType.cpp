// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTService_PhaseType.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Components/CHealthPointComponent.h"
#include "Characters/CEnemy_AI_Boss.h"

UCBTService_PhaseType::UCBTService_PhaseType()
{
	NodeName = "PhaseType";

	Interval = 0.1f;
	RandomDeviation = 0.0f;

}

void UCBTService_PhaseType::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI_Boss* boss = Cast<ACEnemy_AI_Boss>(controller->GetPawn());
	CheckNull(boss);

	//boss의 behavior컴포넌트 가져오기
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(boss);
	CheckNull(behavior);

	UCHealthPointComponent* health = CHelpers::GetComponent<UCHealthPointComponent>(boss);
	CheckNull(health);

	//OnePhaseMin=200, TwoPhseMin = 150, ThreePhaseMin=80
	//CurrHealth = 130 

	EPhase CurrentPhase = behavior->CurrentPhaseType();

	if (health->GetHealth() <= OnePhaseMinHP && CurrentPhase==EPhase::OnePhase)
	{
		behavior->SetTwoPhase();
		boss->UpdateHealthColor_TwoPhase();
		return;
	}


	if (health->GetHealth() <= TwoPhaseMinHP && CurrentPhase == EPhase::TwoPhase)
	{
		behavior->SetThreePhase();
		boss->UpdateHealthColor_ThreePhase();
		return;
	}

	if(health->GetHealth()>OnePhaseMinHP)
	{
		behavior->SetOnePhase();
		return;
	}
}