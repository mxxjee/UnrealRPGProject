// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Weapons/CSkillAction.h"
#include "CAnimNotify_EndSkillAction.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCAnimNotify_EndSkillAction : public UAnimNotify
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
		ESkillNum SkillType;

	UPROPERTY(EditAnywhere)
		bool bSpecial;

	UPROPERTY(EditAnywhere)
		bool bFinal;

	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;


};
