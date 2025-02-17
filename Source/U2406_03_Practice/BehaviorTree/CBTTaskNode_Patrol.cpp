// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CBTTaskNode_Patrol.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"
#include "CPatrolPath.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CEquipment.h"
#include "NavigationSystem.h"


UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";

	bNotifyTick = true;	//tick실행여부
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	controller->ClearFocus(EAIFocusPriority::Gameplay);

	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	//지정스플라인 따라서 이동
	if (!!ai->GetPatrolPath())
	{
		FVector moveToPoint = ai->GetPatrolPath()->GetMoveTo();
		behavior->SetPatrolLocation(moveToPoint);

		if (bDebugMode)
			DrawDebugSphere(ai->GetWorld(), moveToPoint, 10, 10, FColor::Green, true, 5);

		return EBTNodeResult::InProgress;
		
	}


	return EBTNodeResult::InProgress;//진행중임 리턴, 움직이는 도중에 끝내면안됨.

}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	//가려는 위치가져오기
	FVector location = behavior->GetPatrolLocation();

	EPathFollowingRequestResult::Type result = controller->MoveToLocation(location, GoalDistance, false);

	switch (result)
	{
		case EPathFollowingRequestResult::Failed:
		{
			//태스크종료,실패
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			break;
		}

		case EPathFollowingRequestResult::AlreadyAtGoal:
		{
			if (!!ai->GetPatrolPath())
				ai->GetPatrolPath()->UpdateIndex();

			//태스크종료,성공
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			break;
		}


	}
}

