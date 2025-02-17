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
		bool bDebugMode;	//true시 waypoint 보임.

	UPROPERTY(EditAnywhere, Category = "Patrol")
		float GoalDistance = 20;	//간격안에 들면 도착했다고 판단

	UPROPERTY(EditAnywhere, Category = "Patrol")
		float RandomRadius = 1500;	//waypoint가 없는 애는 랜덤반경안에서 위치를찾아 이동

public:
	UCBTTaskNode_Patrol();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
};
