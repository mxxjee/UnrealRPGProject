// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "CAnimNotify_FootSteps.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FFootStepType
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY(EditAnywhere)
		class USoundCue* sound;

};
UCLASS()
class U2406_03_PRACTICE_API UCAnimNotify_FootSteps : public UAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Types")
		FFootStepType Stone;

	UPROPERTY(EditAnywhere, Category = "Types")
		FFootStepType Dirt;

	UPROPERTY(EditAnywhere, Category = "Types")
		FFootStepType Water;

	UPROPERTY(EditAnywhere, Category = "Types")
		FFootStepType Gravel;

	UPROPERTY(EditAnywhere, Category = "Types")
		FFootStepType Carpet;
private:
	FString GetNotifyName_Implementation() const override;
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
