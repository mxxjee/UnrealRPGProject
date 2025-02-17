// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Random.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class U2406_03_PRACTICE_API UCDoAction_Random : public UCDoAction
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
		bool bUseKnockDownAttack=false;
public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

public:
	void OnAttachmentEndCollision() override;

	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther, FHitResult InResult, UPrimitiveComponent* OverlappedComponent, UPrimitiveComponent* OtherComp) override;
	void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) override;

private:
	void SetPlayerGuardCancle();
private:
	int32 Index=-1;
	class ACAIController* controller;
	bool bIsKnockdownAction;
	int AttackCount;
	int MaxAttackCount =5 ;

	class ACPlayer* Targetplayer;
};
