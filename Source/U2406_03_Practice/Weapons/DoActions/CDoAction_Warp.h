// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Warp.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCDoAction_Warp : public UCDoAction
{
	GENERATED_BODY()
	
public:

	UCDoAction_Warp();

	void BeginPlay
	(
		class ACharacter* InOwner,
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		const TArray<FDoActionData>& InDoActionDatas,
		const TArray<FHitData>& InHitDatas
	) override;

	void Tick(float InDeltaTime) override;

public:
	void DoAction() override;
	void Begin_DoAction() override;

private:
	bool GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation);
private:
	class UDecalComponent* Decal;
	class APlayerController* PlayerController;
	class UCAIBehaviorComponent* Behavior;

private:
	FVector MoveToLocation;
};
