// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Combo.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCDoAction_Combo : public UCDoAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Settings")
		float RInterpTime = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
		float FinishAngle = 5.f;

public:
	void Tick(float InDeltaTime) override;
public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

	void AbortAction() override;

public:
	void DestroyGhostTrail();

public:
	void OnAttachmentEndCollision() override;

	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther, FHitResult InResult, UPrimitiveComponent* OverlappedComponent, UPrimitiveComponent* OtherComp) override;
	void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) override;

private:
	int32 Index;

	bool bEnable;
	bool bExist;
	
private:
	TArray<class ACharacter*> Hitted;
	FRotator target;
	FRotator OwnerToTarget;
	class ACharacter* candidate = nullptr;
};
