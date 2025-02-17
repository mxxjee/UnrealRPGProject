// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Dead.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCBTTaskNode_Dead : public UBTTaskNode
{
	GENERATED_BODY()
	

public:
	UCBTTaskNode_Dead();


protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
