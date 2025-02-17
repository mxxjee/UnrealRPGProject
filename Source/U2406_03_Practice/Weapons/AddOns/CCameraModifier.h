// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraModifier.h"
#include "CCameraModifier.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCCameraModifier : public UCameraModifier
{
	GENERATED_BODY()
	
public:
	//외부에서 값을 넣어줄 수 있게 FORCEINLINE 함수 사용.
	FORCEINLINE void SetLocation(const FVector& InLocation) { Location = InLocation; }
	FORCEINLINE void SetRotation(const FRotator& InRotation) { Rotation = InRotation; }

protected:
	void ModifyCamera(float DeltaTime, FVector ViewLocation, FRotator ViewRotation, float FOV, FVector& NewViewLocation, FRotator& NewViewRotation, float& NewFOV) override;

private:
	FVector Location;
	FRotator Rotation;
};
