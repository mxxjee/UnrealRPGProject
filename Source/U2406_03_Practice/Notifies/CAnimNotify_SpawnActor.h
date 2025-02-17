// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_SpawnActor.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCAnimNotify_SpawnActor : public UAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> ToSpawnActor;
	
	UPROPERTY(EditAnywhere)
		FVector OffSet;

	UPROPERTY(EditAnywhere)
		FRotator RotateOffSet;


	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
