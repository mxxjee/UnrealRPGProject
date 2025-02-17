// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Patrol.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCBTTaskNode_Patrol : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Patrol")
		bool bDebugMode;	//true�� waypoint ����.

	UPROPERTY(EditAnywhere, Category = "Patrol")
		float GoalDistance = 20;	//���ݾȿ� ��� �����ߴٰ� �Ǵ�

	UPROPERTY(EditAnywhere, Category = "Patrol")
		float RandomRadius = 1500;	//waypoint�� ���� �ִ� �����ݰ�ȿ��� ��ġ��ã�� �̵�

public:
	UCBTTaskNode_Patrol();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
};
