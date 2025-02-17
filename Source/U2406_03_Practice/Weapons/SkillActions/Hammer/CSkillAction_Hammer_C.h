// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSkillAction.h"
#include "CSkillAction_Hammer_C.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class U2406_03_PRACTICE_API UCSkillAction_Hammer_C : public UCSkillAction
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Options")
		TArray<FDoActionData> NewDoActionDatas;

	UPROPERTY(EditDefaultsOnly, Category = "Options")
		TArray<FHitData> NewHitDatas;

private:
	TArray<FDoActionData> InitDoActionDatas;
	TArray<FHitData> InitHitDatas;

private:
	void DoSkillAction() override;

	void Begin_DoSkillAction() override;
	void End_DoSkillAction() override;

public:
	void OnAttachmentEndCollision() override;
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther, FHitResult InResult, UPrimitiveComponent* OverlappedComponent, UPrimitiveComponent* OtherComp) override;

};
