// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CWeaponComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "CStateComponent.h"
#include "Weapons/CWeaponAsset.h"
#include "Weapons/CWeaponData.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CSkillAction.h"
// Sets default values for this component's properties
UCWeaponComponent::UCWeaponComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;


}

void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	for (int32 i = 0;i < (int32)EWeaponType::Max;i++)
	{
		if (!!DataAssets[i])
		{
			DataAssets[i]->BeginPlay(OwnerCharacter, &Datas[i]);
		}
	}
	
}


void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!!GetDoAction())
		GetDoAction()->Tick(DeltaTime);

	if (!!GetSubAction() && !!GetAttachment())
	{
		GetSubAction()->Tick(DeltaTime);
	}

	CheckNull(DataAssets[0]); 
	CheckTrue(GetSkillActionsNum()==0)
	if (!!GetSkillAction(0))
		GetSkillAction(0)->Tick(DeltaTime);
}

ACAttachment* UCWeaponComponent::GetAttachment()
{
	CheckTrueResult(IsUnarmedMode(),nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetAttachment();
}

UCEquipment* UCWeaponComponent::GetEquipment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetEquipment();
}

UCDoAction* UCWeaponComponent::GetDoAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetDoAction();
}

UCSubAction* UCWeaponComponent::GetSubAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);	//현재타입의 에셋이없을경우

	return Datas[(int32)Type]->GetSubAction();
}

UCSkillAction* UCWeaponComponent::GetSkillAction(int InSkillNum)
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetSkillAction(InSkillNum);
}

TArray<FHitData> UCWeaponComponent::GetHitDatas()
{
	CheckTrueResult(IsUnarmedMode(), {});
	CheckFalseResult(!!Datas[(int32)Type], {});	//현재타입의 에셋이없을경우
	return DataAssets[(int32)Type]->GetFHitDatas();
}

TArray<FDoActionData> UCWeaponComponent::GetDoActionDatas()
{
	CheckTrueResult(IsUnarmedMode(), {});
	CheckFalseResult(!!Datas[(int32)Type], {});	//현재타입의 에셋이없을경우
	return DataAssets[(int32)Type]->GetFDoActionDatas();
}


int UCWeaponComponent::GetSkillActionsNum()
{
	CheckTrueResult(IsUnarmedMode(), 0);
	return DataAssets[(int32)Type]->GetSkillActionsNum();
}


void UCWeaponComponent::SetUnarmedMode()
{
	GetEquipment()->Unequip();
	ChangeType(EWeaponType::Max);
}

bool UCWeaponComponent::IsIdleMode()
{
	return CHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsIdleMode();
}

bool UCWeaponComponent::IsSwordMode()
{
	return (Type == EWeaponType::Sword);
}

bool UCWeaponComponent::IsHammerMode()
{
	return (Type == EWeaponType::Hammer);
}

bool UCWeaponComponent::IsBowMode()
{
	return (Type == EWeaponType::Bow);
}

void UCWeaponComponent::SetFistMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Fist);
}

void UCWeaponComponent::SetSwordMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Sword);
}

void UCWeaponComponent::SetHammerMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Hammer);
}


void UCWeaponComponent::SetWarpMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Warp);
}

void UCWeaponComponent::SetBowMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Bow);
}


void UCWeaponComponent::SetMode(EWeaponType InType)
{
	if (Type == InType)
	{
		SetUnarmedMode();
		return;
	}
	
	else if (IsUnarmedMode() == false)
	{
		//현재장착무기 해제

		GetEquipment()->Unequip();


	}

	if (!!Datas[(int32)InType])
	{
		Datas[(int32)InType]->GetEquipment()->Equip();
		ChangeType(InType);
	}

}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType PrevType = Type;
	Type = InType;

	if (OnWeaponChanged.IsBound())
		OnWeaponChanged.Broadcast(PrevType,InType);

}

void UCWeaponComponent::DoAction()
{
	if (!!GetDoAction())
		GetDoAction()->DoAction();
}

void UCWeaponComponent::SubAction_Pressed()
{
	if (!!GetSubAction())
		GetSubAction()->Pressed();
}

void UCWeaponComponent::SubAction_Released()
{
	if (!!GetSubAction())
		GetSubAction()->Released();
}

void UCWeaponComponent::Skill_Pressed(int InSkillNum)
{
	if (!!GetSkillAction(InSkillNum))
		GetSkillAction(InSkillNum)->Pressed();
}

void UCWeaponComponent::Skill_Released(int InSkillNum)
{
	if (!!GetSkillAction(InSkillNum))
		GetSkillAction(InSkillNum)->Released();
}

void UCWeaponComponent::Skill_Repeat(int InSkillNum)
{
	if (!!GetSkillAction(InSkillNum))
		GetSkillAction(InSkillNum)->Pressing();
}


void UCWeaponComponent::Skill_Canceled(int InSkillNum)
{
	if (!!GetSkillAction(InSkillNum))
		GetSkillAction(InSkillNum)->Cancel_Skill();
}

bool UCWeaponComponent::GetSkillActionIsChargetAttack(int InSkillNum)
{
	CheckNullResult(GetSkillAction(InSkillNum), false);

	return GetSkillAction(InSkillNum)->GetIsChargeSkill();
}