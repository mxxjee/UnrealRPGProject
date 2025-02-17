// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTTaskNode_Dead.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Components/CAIBehaviorComponent.h"

UCBTTaskNode_Dead::UCBTTaskNode_Dead()
{
	NodeName = "Dead";
}

EBTNodeResult::Type UCBTTaskNode_Dead::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(OwnerComp.GetOwner());

	controller->StopMovement();
	controller->ClearFocus(EAIFocusPriority::Gameplay);

	return EBTNodeResult::Succeeded;
}
