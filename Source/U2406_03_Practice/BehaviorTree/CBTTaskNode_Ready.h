// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Ready.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCBTTaskNode_Ready : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_Ready();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


};
