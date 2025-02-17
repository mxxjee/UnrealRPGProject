// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSkillAction.h"
#include "CSkillAction_Wizard_Spawn.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class U2406_03_PRACTICE_API UCSkillAction_Wizard_Spawn : public UCSkillAction
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<class ACObstacle>> ObStaclesClass;


private:
	void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment) override;

private:
	void DoSkillAction() override;

	void Begin_DoSkillAction() override;
	void End_DoSkillAction() override;

private:
	class UCAIBehaviorComponent* Behavior;

public:
	void ShowDecals();
	void SpawnObstacles();

private:
	void SetSpawnLocs();
	
private:
	TArray<FVector> SpawnLocs;
	TArray<class ACSpawnLoc*> spawns;
};
