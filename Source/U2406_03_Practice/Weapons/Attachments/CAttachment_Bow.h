// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttachment.h"
#include "CAttachment_Bow.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API ACAttachment_Bow : public ACAttachment
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "View")
		FVector2D ViewPitchRange = FVector2D(-40, 30);

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class USkeletalMeshComponent* SkeletalMesh;

private:
	UPROPERTY(VisibleAnywhere)
		class UPoseableMeshComponent* PoseableMesh;	//잔상효과를 위한 컴포넌트.
public:
	ACAttachment_Bow();
	
public:
	void OnBeginEquip_Implementation() override;
	void OnUnEquip_Implementation() override;
protected:
	void BeginPlay() override;


private:
	FVector2D OriginViewPitchRange;

public:
	float* GetAnimInstance_Bending();
};
