// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSkillAction.h"
#include "CSkillAction_Sword_Parry.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class U2406_03_PRACTICE_API UCSkillAction_Sword_Parry : public UCSkillAction
{
	GENERATED_BODY()
	
public:
	UCSkillAction_Sword_Parry();
private:



	UPROPERTY(EditAnywhere, Category = "Particle")
		class UNiagaraSystem* Particle;

	UPROPERTY(EditAnywhere, Category = "Particle")
		class UNiagaraSystem* Particle_Slash;


	UPROPERTY(EditAnywhere, Category = "Particle")
		FVector EffectScale = FVector(1, 1, 1);

	UPROPERTY(EditAnywhere)
		float ReturnTime;

	UPROPERTY(EditAnywhere)
		float TimeDilation = 0.001f;


	UPROPERTY(EditAnywhere, Category = "Trace")
		float extentX;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float extentY;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float extentZ;
public:
	void SlowTime() override;
	void OnBoxTrace() override;

	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment) override;
	virtual void Tick(float InDeltaTime) override;
private:
	void DoSkillAction() override;
	void End_DoSkillAction() override;

public:
	void SetStopSpawnGhostTrail();
private:
	class ACAttachment* enemyweapon;
	bool bBlocking;
	TArray<ACharacter*> characters;
	FHitResult hitResult;
	TArray<AActor*> ignores;
	float DeltaTime;


	class UCameraComponent* Camera;
	class USpringArmComponent* SpringArm;
};
