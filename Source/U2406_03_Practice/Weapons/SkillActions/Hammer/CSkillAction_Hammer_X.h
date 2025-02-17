// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSkillAction.h"
#include "CSkillAction_Hammer_X.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class U2406_03_PRACTICE_API UCSkillAction_Hammer_X : public UCSkillAction
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Value")
		float LaunchGravityScale = 0.7f;

	UPROPERTY(EditAnywhere, Category = "Value")
		float LaunchSpeed;

	UPROPERTY(EditAnywhere, Category = "Value")
		float RecoverGravityScale;

	UPROPERTY(EditAnywhere, Category = "Value")
		float StopTime;

	UPROPERTY(EditAnywhere, Category = "Enemy")
		float EnemyGravityScale = 0.7f;

	UPROPERTY(EditAnywhere, Category = "Enemy")
		float EnemyLaunchSpeed;

	UPROPERTY(EditAnywhere, Category = "Enemy")
		float MidDistance;

	UPROPERTY(EditAnywhere, Category = "Enemy")
		float MinDistance;

	UPROPERTY(EditAnywhere, Category = "Enemy")
		float EnemyRecoverGravityScale;

	UPROPERTY(EditAnywhere, Category = "Enemy")
		float EnemyStopTime;

	void DoSkillAction() override;

	void Begin_DoSkillAction() override;
	void End_DoSkillAction() override;


	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther, FHitResult InResult, UPrimitiveComponent* OverlappedComponent, UPrimitiveComponent* OtherComp) override;

public:
	FORCEINLINE bool GetbUseHammerXSkill() { return bUseHammerXSkill; }
public:
	void TakeLaunch();		//Enemy¸¦ ¶ç¿ò
	void LaunchPlayer();		//Player¸¦ ¶ç¿ò

	void StopLaunchPlayer();

private:
	bool bUseHammerXSkill=false;
	float Distance;
};
