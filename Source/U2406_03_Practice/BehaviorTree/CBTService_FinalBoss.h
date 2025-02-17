// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_FinalBoss.generated.h"


UCLASS()
class U2406_03_PRACTICE_API UCBTService_FinalBoss : public UBTService
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Action")
		float AvoidRange = 150;

	UPROPERTY(EditAnywhere, Category = "Action")
		bool bDrawDebug;


public:
	UCBTService_FinalBoss();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
