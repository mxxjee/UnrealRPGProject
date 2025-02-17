// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CWeaponAsset.h"
#include "Global.h"
#include "CAttachment.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CDoAction.h"
#include "GameFramework/Character.h"
#include "Weapons/CSubAction.h"
#include "Weapons/CSkillAction.h"
#include "CWeaponData.h"

UCWeaponAsset::UCWeaponAsset()
{
	//지정해주지 않는 경우도 있기에 기본형지정.

	AttachmentClass = ACAttachment::StaticClass();
	EquipmentClass = UCEquipment::StaticClass();

}



void UCWeaponAsset::BeginPlay(ACharacter* InOwner, UCWeaponData** OutWeaponData)
{
	ACAttachment* attachment = nullptr;
	if (!!AttachmentClass)
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;

		//지정한 attachmentclass타입을 생성시킨다.
		attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);
	}

	UCEquipment* equipment = nullptr;
	if (!!EquipmentClass)
	{
		//직렬화로 관리되는 아이들 동적할당방법
		equipment = NewObject<UCEquipment>(this, EquipmentClass);
		equipment->BeginPlay(InOwner, EquipmentData);


		if (!!attachment) //생성 후 이벤트연결
		{
			equipment->OnEquipmentBeginEquip.AddDynamic(attachment, &ACAttachment::OnBeginEquip);
			equipment->OnEquipmentUnequip.AddDynamic(attachment, &ACAttachment::OnUnEquip);
		}
	}

	UCDoAction* doAction = nullptr;
	if (!!DoActionClass)
	{
		doAction = NewObject<UCDoAction>(this, DoActionClass);
		doAction->BeginPlay(InOwner, attachment, equipment, DoActionDatas, FHitDatas);

		if (!!attachment)
		{
			attachment->OnAttachmentBeginCollision.AddDynamic(doAction, &UCDoAction::OnAttachmentBeginCollision);
			attachment->OnAttachmentEndCollision.AddDynamic(doAction, &UCDoAction::OnAttachmentEndCollision);

			attachment->OnAttachmentBeginOverlap.AddDynamic(doAction, &UCDoAction::OnAttachmentBeginOverlap);
			attachment->OnAttachmentEndOverlap.AddDynamic(doAction, &UCDoAction::OnAttachmentEndOverlap);

		}

		if (!!equipment)
		{
			equipment->OnEquipmentBeginEquip.AddDynamic(doAction, &UCDoAction::OnBeginEquip);
			equipment->OnEquipmentUnequip.AddDynamic(doAction, &UCDoAction::OnUnequip);
		}
	}


	UCSubAction* subAction = nullptr;
	if (!!SubActionClass)
	{
		subAction = NewObject<UCSubAction>(this, SubActionClass);
		subAction->BeginPlay(InOwner, attachment, doAction);

	}

	*OutWeaponData = NewObject<UCWeaponData>();
	
	/*각 스킬마다 클래스생성*/
	if (SkillActionsClass.Num() > 0)
	{
		for (TSubclassOf<UCSkillAction> skill : SkillActionsClass)
		{
			if (!!skill)
			{
				UCSkillAction* SkillObj = nullptr;

				SkillObj = NewObject<UCSkillAction>(this, skill);
				SkillObj->BeginPlay(InOwner, attachment);

				if (!!attachment)
				{
					attachment->OnAttachmentEndCollisionSkill.AddDynamic(SkillObj, &UCSkillAction::OnAttachmentEndCollision);
					attachment->OnAttachmentBeginOverlapSkill.AddDynamic(SkillObj, &UCSkillAction::OnAttachmentBeginOverlap);
					attachment->OnAttachmentEndOverlapSkill.AddDynamic(SkillObj, &UCSkillAction::OnAttachmentEndOverlap);
					
				}

				(*OutWeaponData)->SkillActions.Add(SkillObj);
			}


		}
	}
	
	
	(*OutWeaponData)->Attachment = attachment;
	(*OutWeaponData)->Equipment = equipment;
	(*OutWeaponData)->DoAction = doAction;
	(*OutWeaponData)->SubAction = subAction;
}
