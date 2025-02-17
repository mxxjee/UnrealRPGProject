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
	//���������� �ʴ� ��쵵 �ֱ⿡ �⺻������.

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

		//������ attachmentclassŸ���� ������Ų��.
		attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);
	}

	UCEquipment* equipment = nullptr;
	if (!!EquipmentClass)
	{
		//����ȭ�� �����Ǵ� ���̵� �����Ҵ���
		equipment = NewObject<UCEquipment>(this, EquipmentClass);
		equipment->BeginPlay(InOwner, EquipmentData);


		if (!!attachment) //���� �� �̺�Ʈ����
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
	
	/*�� ��ų���� Ŭ��������*/
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
