// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTService_HitType.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"

UCBTService_HitType::UCBTService_HitType()
{
	NodeName = "HitType";

	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UCBTService_HitType::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(ai);

	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	CheckNull(behavior);


	if (ai->GetbAirSkillDamaged())
		behavior->SetHitTypeAirMode();

	else
		behavior->SetHitTypeNormalMode();

}