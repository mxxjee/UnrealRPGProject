// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSkillAction.h"
#include "CSkillAction_Sword_C.generated.h"

/**
 * 
 */

UCLASS(Blueprintable)
class U2406_03_PRACTICE_API UCSkillAction_Sword_C : public UCSkillAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		float LaunchGravityScale=0.7f;

	UPROPERTY(EditAnywhere)
		float EnemyLaunchGravityScale = 0.8f;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* DamagedMontage;

	UPROPERTY(EditAnywhere)
		float RecoverGravityScale;

	UPROPERTY(EditAnywhere)
		float RecoverEnemyGravityScale;

	UPROPERTY(EditAnywhere)
		float FinalLaunch;

	UPROPERTY(EditAnywhere)
		float LaunchSpeed;


public:
	FORCEINLINE bool GetbUseSwordSkill() { return bUseSwordCSkill; }
	FORCEINLINE void SetbUseSwordSkill(bool bInUseSwordSkill) { bUseSwordCSkill= bInUseSwordSkill; }
public:
	void LaunchPlayer();
	void SetStopSpawnGhostTrail();
	void StopLaunchEnemy();
	void StopLaunchPlayer();

private:
	void DoSkillAction() override;

	void Begin_DoSkillAction() override;
	void End_DoSkillAction() override;
	
public:
	void OnAttachmentEndCollision() override;
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther, FHitResult InResult, UPrimitiveComponent* OverlappedComponent, UPrimitiveComponent* OtherComp) override;
	void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) override;

public:
	void TakeLaunch();

private:
	int Index=-1;
	bool bUseSwordCSkill;

private:
	bool bLaunchEnemy;
	class ACharacter* Attacker;
	class AActor* AttackCauser;
	TArray<class ACharacter*> Hitted;
	class ACPlayer* player;
};
