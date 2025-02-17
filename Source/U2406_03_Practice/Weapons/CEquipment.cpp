// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CEquipment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"


void UCEquipment::BeginPlay(class ACharacter* InOwner, const FEquipmentData& InData)
{
	OwnerCharacter = InOwner;
	Data = InData;

	Movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);
	State = CHelpers::GetComponent<UCStateComponent>(InOwner);

}

void UCEquipment::Equip_Implementation()
{
	State->SetEquipMode();

	if (OnEquipmentEquip.IsBound())
		OnEquipmentEquip.Broadcast();

	if (Data.bCanMove == false)
		Movement->Stop();

	if (Data.bUsedControlRotation)
		Movement->EnableControlRotation();

	if (!!Data.Montage)
	{
		OwnerCharacter->PlayAnimMontage(Data.Montage, Data.PlayRate);
	}
		

	else
	{
		Begin_Equip();	//몽타주가 없으니까 직접호출
		End_Equip();
	}
}

void UCEquipment::Begin_Equip_Implementation()
{
	bBeginEquip = true;



	if (OnEquipmentBeginEquip.IsBound())
		OnEquipmentBeginEquip.Broadcast();
}

void UCEquipment::End_Equip_Implementation()
{
	bBeginEquip = false;
	bEquipped = true;


	if (Data.bCanMove == false)
		Movement->Move();

	State->SetIdleMode();

}
void UCEquipment::Unequip_Implementation()
{
	bEquipped = false;

	if (!!Data.UnEquipMontage)
	{
		OwnerCharacter->PlayAnimMontage(Data.UnEquipMontage);
	}

	if (Data.bUsedControlRotation)
		Movement->DisableControlRotation();

	if (OnEquipmentUnequip.IsBound())
		OnEquipmentUnequip.Broadcast();
}