// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_SetLanceLocation.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CSkillAction.h"
#include "Weapons/SkillActions/CSkillAction_Wizard_Final.h"

FString UCAnimNotify_SetLanceLocation::GetNotifyName_Implementation() const
{
	return "SetLance";
}


void UCAnimNotify_SetLanceLocation::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	CheckNull(weapon->GetSkillAction(4));
	UCSkillAction_Wizard_Final* FianlSkill = Cast<UCSkillAction_Wizard_Final>(weapon->GetSkillAction(4));
	CheckNull(FianlSkill);

	FianlSkill->SetLanceLocation();




}


