// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTTaskNode_Ready.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CAIController.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Characters/CEnemy_AI.h"

UCBTTaskNode_Ready::UCBTTaskNode_Ready()
{
	NodeName = "Ready";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Ready::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	CheckNullResult(controller, EBTNodeResult::Failed);



	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Ready::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);

	

	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	ACharacter* target = behavior->GetTarget();

	CheckNull(target);
	CheckNull(state);

	if (state->IsDeadMode())
	{
		controller->ClearFocus(EAIFocusPriority::Default);
		behavior->SetDeadMode();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	controller->SetFocus(target);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
