// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_TakeDamage.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/SkillActions/Sword/CSkillAction_Sword_X.h"
#include "Weapons/SkillActions/Sword/CSkillAction_Sword_C.h"

FString UCAnimNotify_TakeDamage::GetNotifyName_Implementation() const
{
	return "TakeDamage";
}

void UCAnimNotify_TakeDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	switch (SkillType)
	{

		case ESkillNum::X:
		{
			CheckNull(weapon->GetSkillAction(1));
			UCSkillAction_Sword_X* skill = Cast<UCSkillAction_Sword_X>(weapon->GetSkillAction(1));
			CheckNull(skill);

			skill->TakeDamage();
			break;
		}
			

		case ESkillNum::C:
		{
			CheckNull(weapon->GetSkillAction(2));
			UCSkillAction_Sword_C* skill = Cast<UCSkillAction_Sword_C>(weapon->GetSkillAction(2));
			CheckNull(skill);

			skill->TakeLaunch();
			break;
		}
			


	}
}
