// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_Launch.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCAnimNotify_Launch : public UAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		ESkillNum SkillType;

	UPROPERTY(EditAnywhere)
		bool bHammerXSkill;

	UPROPERTY(EditAnywhere)
		bool bBowXSkill;
private:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
