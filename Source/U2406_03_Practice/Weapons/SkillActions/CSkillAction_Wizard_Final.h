// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSkillAction.h"
#include "CSkillAction_Wizard_Final.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class U2406_03_PRACTICE_API UCSkillAction_Wizard_Final : public UCSkillAction
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACShockWave> ShockWaveClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACLance> lanceClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> Decal;

private:
	void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment) override;

private:
	void DoSkillAction() override;

	void Begin_DoSkillAction() override;
	void End_DoSkillAction() override;

private:
	class UCAIBehaviorComponent* Behavior;

public:
	void SpawnShockWave();
	void SpawnLance();

	void SetLanceLocation();
private:
	FVector LanceLocation;
};
