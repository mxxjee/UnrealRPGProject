// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_StopSpawnGhostTrail.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/SkillActions/Sword/CSkillAction_Sword_X.h"
#include "Weapons/SkillActions/Sword/CSkillAction_Sword_C.h"
#include "Weapons/SkillActions/Sword/CSkillAction_Sword_Parry.h"

FString UCAnimNotify_StopSpawnGhostTrail::GetNotifyName_Implementation() const
{
	return "StopSpawnGhostTrail";
}

void UCAnimNotify_StopSpawnGhostTrail::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);


	if (SwordParry)
	{
		CheckNull(weapon->GetSkillAction(3));
		UCSkillAction_Sword_Parry* skill = Cast<UCSkillAction_Sword_Parry>(weapon->GetSkillAction(3));
		CheckNull(skill);
		skill->SetStopSpawnGhostTrail();
		return;
	}
	switch (SkillType)
	{

		case ESkillNum::X:
		{
			CheckNull(weapon->GetSkillAction(1));
			UCSkillAction_Sword_X* skill = Cast<UCSkillAction_Sword_X>(weapon->GetSkillAction(1));
			CheckNull(skill);

			skill->SetStopSpawnGhostTrail();
			break;
		}

		case ESkillNum::C:
		{
			CheckNull(weapon->GetSkillAction(2));
			UCSkillAction_Sword_C* skill = Cast<UCSkillAction_Sword_C>(weapon->GetSkillAction(2));
			CheckNull(skill);

			skill->SetStopSpawnGhostTrail();
			break;
		}
	
	}
}
