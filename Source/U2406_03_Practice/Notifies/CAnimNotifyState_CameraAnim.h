// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_CameraAnim.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCAnimNotifyState_CameraAnim : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UCAnimNotifyState_CameraAnim();//생성자

private:

	UPROPERTY(EditAnywhere, Category = "Camera")
		float StopTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
		float MoveTime = 0.0f;//원래 카메라에서 Animation 카메라로 전환되는 시간

	UPROPERTY(EditAnywhere, Category = "Camera")
		float RotateTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
		float MoveEndTime = 0.0f;// Animation 카메라->원래카메라

	UPROPERTY(EditAnywhere, Category = "Camera")
		float RotateEndTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
		FVector MovePos=FVector(458.595471f, -272.3516f, 81.851849f);

	UPROPERTY(EditAnywhere, Category = "Camera")
		FRotator MoveRot = FRotator(-10.000000, 124.999999, 0.000000);

public:
	FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	FVector InitPosition;
	FRotator InitRotation;

	class UCameraComponent* camera;
	
	bool bMoveEnd=true;
};
