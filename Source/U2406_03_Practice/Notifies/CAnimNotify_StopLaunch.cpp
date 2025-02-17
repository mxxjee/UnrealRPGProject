// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_StopLaunch.h"
#include "Global.h"
#include "Characters/IStatable.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/SkillActions/Sword/CSkillAction_Sword_C.h"
#include "Weapons/SkillActions/Hammer/CSkillAction_Hammer_X.h"
#include "Weapons/SkillActions/Bow/CSkillAction_Bow_X.h"

FString UCAnimNotify_StopLaunch::GetNotifyName_Implementation() const
{
	return "StopLaunch";
}

void UCAnimNotify_StopLaunch::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	if (bBowXSkill)
	{
		CheckNull(weapon->GetSkillAction(1));
		UCSkillAction_Bow_X* skill = Cast<UCSkillAction_Bow_X>(weapon->GetSkillAction(1));
		CheckNull(skill);

		skill->StopLaunchPlayer();
		return;
	}

	switch (SkillType)
	{
		case ESkillNum::X:
		{
			CheckNull(weapon->GetSkillAction(1));
			UCSkillAction_Hammer_X* skill = Cast<UCSkillAction_Hammer_X>(weapon->GetSkillAction(1));
			CheckNull(skill);

			/*if (Type == LaunchType::ENEMY)
				skill->StopLaunchEnemy();*/

			if (Type == LaunchType::PLAYER)
				skill->StopLaunchPlayer();
			break;
		}
		case ESkillNum::C:
		{
			CheckNull(weapon->GetSkillAction(2));
			UCSkillAction_Sword_C* skill = Cast<UCSkillAction_Sword_C>(weapon->GetSkillAction(2));
			CheckNull(skill);

			if (Type == LaunchType::ENEMY)
				skill->StopLaunchEnemy();

			else if (Type == LaunchType::PLAYER)
				skill->StopLaunchPlayer();
			break;
		}



	}



}

