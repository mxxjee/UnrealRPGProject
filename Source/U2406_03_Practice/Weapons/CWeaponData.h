// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponData.generated.h"


UCLASS()
class U2406_03_PRACTICE_API UCWeaponData : public UObject
{
	GENERATED_BODY()
	
private:
	friend class UCWeaponAsset;
public:
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE class UCDoAction* GetDoAction() { return DoAction; }
	FORCEINLINE class UCSubAction* GetSubAction() { return SubAction; }
	FORCEINLINE class UCSkillAction* GetSkillAction(int InNumber) { return SkillActions[InNumber]; }
private:
	UPROPERTY()
		class ACAttachment* Attachment;

	UPROPERTY()
		class UCEquipment* Equipment;

	UPROPERTY()
		class UCDoAction* DoAction;

	UPROPERTY()
		class UCSubAction* SubAction;

	UPROPERTY()
		TArray<UCSkillAction*> SkillActions;
};
