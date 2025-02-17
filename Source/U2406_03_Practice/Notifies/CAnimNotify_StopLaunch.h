// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Weapons/CSkillAction.h"
#include "CAnimNotify_StopLaunch.generated.h"

UENUM(BlueprintType)
enum class LaunchType : uint8
{
	PLAYER,
	ENEMY
};
UCLASS()
class U2406_03_PRACTICE_API UCAnimNotify_StopLaunch : public UAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		bool bBowXSkill;
	UPROPERTY(EditAnywhere)
		ESkillNum SkillType;

	UPROPERTY(EditAnywhere)
		LaunchType Type;
private:

	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
