// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTTaskNode_AirHit.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

UCBTTaskNode_AirHit::UCBTTaskNode_AirHit()
{
	NodeName = "UCBTTaskNode_AirHit";
	bNotifyTick = true;	//tick실행여부
}

EBTNodeResult::Type UCBTTaskNode_AirHit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	controller = Cast<ACAIController>(OwnerComp.GetOwner());

	controller->StopMovement();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_AirHit::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	CheckNull(controller);
	ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(ai);

	state = CHelpers::GetComponent<UCStateComponent>(ai);
	CheckNull(state);

	if (state->IsIdleMode())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}