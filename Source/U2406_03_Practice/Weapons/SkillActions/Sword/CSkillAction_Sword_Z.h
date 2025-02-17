// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSkillAction.h"
#include "CSkillAction_Sword_Z.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class U2406_03_PRACTICE_API UCSkillAction_Sword_Z : public UCSkillAction
{
	GENERATED_BODY()
	

private:
	UPROPERTY(EditAnywhere, Category = "Settings")
		float RInterpTime = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
		float FinishAngle = 1.2f;

public:
	void Tick(float DeltaTime) override;
public:
	void DoSkillAction() override;

	void Begin_DoSkillAction() override;
	//void OnAttachmentBeginCollision() override;
	void OnAttachmentEndCollision() override;
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther, FHitResult InResult, UPrimitiveComponent* OverlappedComponent, UPrimitiveComponent* OtherComp) override;
	void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) override;

private:
	TArray<class ACharacter*> Hitted;
	TArray<class ACharacter*> SwordHitted;

	FRotator target;
	FRotator OwnerToTarget;
	class ACharacter* candidate = nullptr;
};
