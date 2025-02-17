// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_AirHit.generated.h"

UENUM(BlueprintType)
enum class EType : uint8
{
	PLAYER,
	ENEMY
};

UCLASS()
class U2406_03_PRACTICE_API UCAnimNotify_AirHit : public UAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		EType Type;

private:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
