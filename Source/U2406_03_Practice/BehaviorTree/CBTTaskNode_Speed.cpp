// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTTaskNode_Speed.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"



UCBTTaskNode_Speed::UCBTTaskNode_Speed()
{
	NodeName = "Speed";

}

EBTNodeResult::Type UCBTTaskNode_Speed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	//움직임 : movementcomponent
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(ai);
	movement->SetSpeed(Type);


	return EBTNodeResult::Succeeded;  //return을 해줘야 task가 완료처리.



}