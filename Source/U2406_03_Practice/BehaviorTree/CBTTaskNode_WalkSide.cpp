// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTTaskNode_WalkSide.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CAIController.h"
#include "Components/CAIBehaviorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/CEnemy_AI.h"
#include "NavigationSystem.h"
#include "CPatrolPath.h"

UCBTTaskNode_WalkSide::UCBTTaskNode_WalkSide()
{
	NodeName = "WalkSide";
	bNotifyTick = true;	//tick실행여부
}

EBTNodeResult::Type UCBTTaskNode_WalkSide::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCharacterMovementComponent* movement = ai->GetCharacterMovement();
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	CheckNullResult(controller, EBTNodeResult::Failed);
	CheckNullResult(movement, EBTNodeResult::Failed);


	return EBTNodeResult::InProgress;

	
}


