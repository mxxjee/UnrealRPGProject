// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_BeginSkillAction.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CSkillAction.h"

FString UCAnimNotify_BeginSkillAction::GetNotifyName_Implementation() const
{
	return "BeginSkillAction";
}

void UCAnimNotify_BeginSkillAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	
	if (bFinal)
	{
		CheckNull(weapon->GetSkillAction(4));
		weapon->GetSkillAction(4)->Begin_DoSkillAction();
		return;
	}


	if (bSpecial)
	{
		CheckNull(weapon->GetSkillAction(3));
		weapon->GetSkillAction(3)->Begin_DoSkillAction();
		return;
	}


	switch (SkillType)
	{
		case ESkillNum::Z:
			CheckNull(weapon->GetSkillAction(0));
			weapon->GetSkillAction(0)->Begin_DoSkillAction();
			break;

		case ESkillNum::X:
			CheckNull(weapon->GetSkillAction(1));
			weapon->GetSkillAction(1)->Begin_DoSkillAction();
			break;

		case ESkillNum::C:
			CheckNull(weapon->GetSkillAction(2));
			weapon->GetSkillAction(2)->Begin_DoSkillAction();
			break;
	}


	
}