// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Action.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCBTTaskNode_Action : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_Action();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	//Task중단시에 호출되는함수
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	class ACAIController* controller;
	class ACEnemy_AI* ai;
	class UCWeaponComponent* weapon;
	class UCStateComponent* state;
};
