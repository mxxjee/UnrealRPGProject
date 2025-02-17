// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_JumpEnd.generated.h"

UENUM(BlueprintType)
enum class EJumpType : uint8
{
	NORMAL,
	AIRCOMBO
};
UCLASS()
class U2406_03_PRACTICE_API UCAnimNotify_JumpEnd : public UAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		EJumpType Type;

	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	
};
