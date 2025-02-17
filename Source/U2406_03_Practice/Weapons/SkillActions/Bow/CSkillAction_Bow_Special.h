// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSkillAction.h"
#include "CSkillAction_Bow_Special.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class U2406_03_PRACTICE_API UCSkillAction_Bow_Special : public UCSkillAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Montage")
		class  USoundWave* LockOnSound;

	UPROPERTY(EditAnywhere, Category = "Montage")
		class UAnimMontage* ShootMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
		TSubclassOf<class ACArrow> ArrowClass;

	UPROPERTY(EditAnywhere, Category = "Particle")
		TSubclassOf<class AActor> SkillParticle;

private:
	void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment) override;
	void DoSkillAction() override;

	void Begin_DoSkillAction() override;
	void End_DoSkillAction() override;

	void Pressing() override;
	void Released() override;
	
public:
	void SpawnArrowToEnemy();
private:
	void CreateArrow();
	class ACArrow* GetArrowAttached();

private:
	void CreateParticleActor();
private:
	UFUNCTION()
		void OnArrowHit(class AActor* InCauser, class ACharacter* InOtherCharacter);

	UFUNCTION()
		void OnEndPlay(class ACArrow* InDestroyActor);
private:
	bool bDoChargeAnim;

	class ACPlayer* player;
	TArray<class ACEnemy*> LockOnEnemys;

	class UCAttackPointComponent* Attack;
	AActor* ParticleActor;
};
