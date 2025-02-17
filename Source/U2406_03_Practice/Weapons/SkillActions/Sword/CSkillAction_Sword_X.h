// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSkillAction.h"
#include "CSkillAction_Sword_X.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class U2406_03_PRACTICE_API UCSkillAction_Sword_X : public UCSkillAction
{
	GENERATED_BODY()
		
private:

	UPROPERTY(EditAnywhere)
		float LaunchSpeed= 2101954.5f;

	UPROPERTY(EditAnywhere)
		float SphereDistance;

private:
	void DoSkillAction() override;
	void End_DoSkillAction() override;

public:
	void TakeDamage();
public:
	void Begin_DoSkillAction() override;
public:
	void OnAttachmentEndCollision() override;
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther, FHitResult InResult, UPrimitiveComponent* OverlappedComponent, UPrimitiveComponent* OtherComp) override;
	void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) override;

public:
	void LaunchPlayer();
	void SetStopSpawnGhostTrail();
private:
	TArray<class ACharacter*> Hitted;
	TArray<FHitResult> hitResults;

	ACharacter* Attacker;
	AActor* AttackCauser;
};
