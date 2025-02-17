// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance_Bow.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCAnimInstance_Bow : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float Bending;


public:
	FORCEINLINE float* GetBending() { return &Bending; }

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;
};
