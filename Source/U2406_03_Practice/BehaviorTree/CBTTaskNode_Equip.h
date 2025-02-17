// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/CWeaponComponent.h"
#include "CBTTaskNode_Equip.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCBTTaskNode_Equip : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Weapon")	//������ ���⼱��
		EWeaponType Type = EWeaponType::Max;

public:
	UCBTTaskNode_Equip();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	//Task�ߴܽÿ� ȣ��Ǵ��Լ�
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
