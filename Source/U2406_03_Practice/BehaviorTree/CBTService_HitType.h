// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_HitType.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCBTService_HitType : public UBTService
{
	GENERATED_BODY()
	
public:
	UCBTService_HitType();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
