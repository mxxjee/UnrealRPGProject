// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_Launch.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CSkillAction.h"
#include "Weapons/SkillActions/Sword/CSkillAction_Sword_X.h"
#include "Weapons/SkillActions/Sword/CSkillAction_Sword_C.h"
#include "Weapons/SkillActions/Hammer/CSkillAction_Hammer_X.h"
#include "Weapons/SkillActions/Bow/CSkillAction_Bow_X.h"

FString UCAnimNotify_Launch::GetNotifyName_Implementation() const
{
	return "Launch";
}

void UCAnimNotify_Launch::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);


	if (bBowXSkill)
	{
		CheckNull(weapon->GetSkillAction(1));
		UCSkillAction_Bow_X* Bow_X = Cast<UCSkillAction_Bow_X>(weapon->GetSkillAction(1));
		CheckNull(Bow_X);

		Bow_X->LaunchPlayer();
		return;
	}


	if (bHammerXSkill)
	{
		CheckNull(weapon->GetSkillAction(1));
		UCSkillAction_Hammer_X* Hammer_X = Cast<UCSkillAction_Hammer_X>(weapon->GetSkillAction(1));
		CheckNull(Hammer_X);

		Hammer_X->LaunchPlayer();
		return;
	}
	switch (SkillType)
	{

		case ESkillNum::X:
		{
			CheckNull(weapon->GetSkillAction(1));
			UCSkillAction_Sword_X* skill_X = Cast<UCSkillAction_Sword_X>(weapon->GetSkillAction(1));
			CheckNull(skill_X);

			skill_X->LaunchPlayer();
			break;
			
		}
		

		case ESkillNum::C:
		{
			CheckNull(weapon->GetSkillAction(2));
			UCSkillAction_Sword_C* skill_C = Cast<UCSkillAction_Sword_C>(weapon->GetSkillAction(2));
			CheckNull(skill_C);

			skill_C->LaunchPlayer();
			break;
		}
			

	}
}

