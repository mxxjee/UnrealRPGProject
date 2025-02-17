// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_EndSkillAction.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CSkillAction.h"

FString UCAnimNotify_EndSkillAction::GetNotifyName_Implementation() const
{
	return "EndSkillAction";
}

void UCAnimNotify_EndSkillAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	if (bFinal)
	{
		CheckNull(weapon->GetSkillAction(4));
		weapon->GetSkillAction(4)->End_DoSkillAction();
		return;
	}

	if (bSpecial)
	{
		CheckNull(weapon->GetSkillAction(3));
		weapon->GetSkillAction(3)->End_DoSkillAction();
		return;

	}
	switch (SkillType)
	{
		case ESkillNum::Z:
			CheckNull(weapon->GetSkillAction(0));
			weapon->GetSkillAction(0)->End_DoSkillAction();
			break;

		case ESkillNum::X:
			CheckNull(weapon->GetSkillAction(1));
			weapon->GetSkillAction(1)->End_DoSkillAction();
			break;

		case ESkillNum::C:
			CheckNull(weapon->GetSkillAction(2));
			weapon->GetSkillAction(2)->End_DoSkillAction();
			break;
	}



}
