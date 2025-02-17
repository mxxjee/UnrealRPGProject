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

	bNotifyTick = true;	//tick���࿩��
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	controller->ClearFocus(EAIFocusPriority::Gameplay);

	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	//�������ö��� ���� �̵�
	if (!!ai->GetPatrolPath())
	{
		FVector moveToPoint = ai->GetPatrolPath()->GetMoveTo();
		behavior->SetPatrolLocation(moveToPoint);

		if (bDebugMode)
			DrawDebugSphere(ai->GetWorld(), moveToPoint, 10, 10, FColor::Green, true, 5);

		return EBTNodeResult::InProgress;
		
	}


	return EBTNodeResult::InProgress;//�������� ����, �����̴� ���߿� ������ȵ�.

}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	//������ ��ġ��������
	FVector location = behavior->GetPatrolLocation();

	EPathFollowingRequestResult::Type result = controller->MoveToLocation(location, GoalDistance, false);

	switch (result)
	{
		case EPathFollowingRequestResult::Failed:
		{
			//�½�ũ����,����
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			break;
		}

		case EPathFollowingRequestResult::AlreadyAtGoal:
		{
			if (!!ai->GetPatrolPath())
				ai->GetPatrolPath()->UpdateIndex();

			//�½�ũ����,����
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			break;
		}


	}
}

