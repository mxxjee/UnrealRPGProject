// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructures.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentBeginEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentUnequip);

UCLASS()
class U2406_03_PRACTICE_API UCEquipment : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE const bool GetBeginEquip() { return bBeginEquip; }
	FORCEINLINE const bool* GetbEquipped() { return &bEquipped; }
public:
	void BeginPlay(class ACharacter* InOwner, const FEquipmentData& InData);

public:
	UFUNCTION(BlueprintNativeEvent)
		void Equip();
	void Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Begin_Equip();
	void Begin_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void End_Equip();
	void End_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void Unequip();
	void Unequip_Implementation();

public:
	FEquipmentEquip OnEquipmentEquip;
	FEquipmentBeginEquip OnEquipmentBeginEquip;
	FEquipmentUnequip OnEquipmentUnequip;

private:
	class ACharacter* OwnerCharacter;

private:
	FEquipmentData Data;


public:
	class UCMovementComponent* Movement;
	class UCStateComponent* State;

public:
	bool bBeginEquip=false;
	bool bEquipped;
};
