// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_MiddleBoss.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCBTService_MiddleBoss : public UBTService
{
	GENERATED_BODY()
	
private:


	UPROPERTY(EditAnywhere, Category = "Range")
		float ApproachRange = 600;

	UPROPERTY(EditAnywhere, Category = "Action")
		float ActionRange = 150;

public:
	UCBTService_MiddleBoss();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
