// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_ShowDecals.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CSkillAction.h"
#include "Weapons/SkillActions/CSkillAction_Wizard_Spawn.h"

FString UCAnimNotify_ShowDecals::GetNotifyName_Implementation() const
{
	return "ShowDecals";
}

void UCAnimNotify_ShowDecals::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	CheckNull(weapon->GetSkillAction(1));
	UCSkillAction_Wizard_Spawn* spawnSkill = Cast<UCSkillAction_Wizard_Spawn>(weapon->GetSkillAction(1));
	CheckNull(spawnSkill);

	spawnSkill->ShowDecals();




}