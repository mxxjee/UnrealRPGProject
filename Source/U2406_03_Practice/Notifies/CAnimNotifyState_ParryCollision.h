// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_ParryCollision.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCAnimNotifyState_ParryCollision : public UAnimNotifyState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		int SkillNum;
private:
	FString GetNotifyName_Implementation() const override;
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
