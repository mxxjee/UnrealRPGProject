// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_Slowmotion.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCAnimNotify_Slowmotion : public UAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		int SkillNum;
public:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
