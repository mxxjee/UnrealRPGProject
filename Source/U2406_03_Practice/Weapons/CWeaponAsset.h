// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapons/CWeaponStructures.h"
#include "CWeaponAsset.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCWeaponAsset : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCEquipment> EquipmentClass;

	UPROPERTY(EditAnywhere)
		FEquipmentData EquipmentData;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCDoAction> DoActionClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UCSubAction> SubActionClass;

	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<class UCSkillAction>> SkillActionsClass;

	UPROPERTY(EditAnywhere)
		TArray<FDoActionData> DoActionDatas;
	
	UPROPERTY(EditAnywhere)
		TArray<FHitData> FHitDatas;


public:
	UCWeaponAsset();

	void BeginPlay(class ACharacter* InOwner,class UCWeaponData** OutWeaponData);

public:
	TArray<FHitData> GetFHitDatas() { return FHitDatas; }
	FORCEINLINE int GetSkillActionsNum() { return SkillActionsClass.Num(); }
	TArray<FDoActionData> GetFDoActionDatas() { return DoActionDatas; }
};
