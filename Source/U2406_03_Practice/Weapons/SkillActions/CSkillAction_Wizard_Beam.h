// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSkillAction.h"
#include "CSkillAction_Wizard_Beam.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class U2406_03_PRACTICE_API UCSkillAction_Wizard_Beam : public UCSkillAction
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACBeam> BeamClass;


private:
	void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment) override;

private:
	void DoSkillAction() override;

	void Begin_DoSkillAction() override;
	void End_DoSkillAction() override;

public:
	void SpawnBeam();
private:
	class UCAIBehaviorComponent* Behavior;
};
