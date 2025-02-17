// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_WalkSide.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCBTTaskNode_WalkSide : public UBTTaskNode
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere)
		float Force;

	UPROPERTY(EditAnywhere)
		bool bDebugMode;
public:
	UCBTTaskNode_WalkSide();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	int bMoveDirection;

	
};
