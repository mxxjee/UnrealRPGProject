// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Components/CAIBehaviorComponent.h"
#include "CBTService_PhaseType.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCBTService_PhaseType : public UBTService
{
	GENERATED_BODY()
	
private:
	//각 페이즈에 도달하기 위한 HP
	UPROPERTY(EditAnywhere, Category = "Range")
		float OnePhaseMinHP = 200;

	UPROPERTY(EditAnywhere, Category = "Range")
		float TwoPhaseMinHP = 150;

	UPROPERTY(EditAnywhere, Category = "Range")
		float ThreePhaseMinHP = 80;

public:
	UCBTService_PhaseType();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
