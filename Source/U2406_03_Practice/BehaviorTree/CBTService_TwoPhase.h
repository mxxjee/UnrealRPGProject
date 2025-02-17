// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_TwoPhase.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCBTService_TwoPhase : public UBTService
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Action")
		float AvoidRange = 1000;

	UPROPERTY(EditAnywhere, Category = "Action")
		float BeamRange = 2000;


	UPROPERTY(EditAnywhere, Category = "Action")
		bool bDrawDebug;

public:
	UCBTService_TwoPhase();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
