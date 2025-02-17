// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_SpawnBeam.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CSkillAction.h"
#include "Weapons/SkillActions/CSkillAction_Wizard_Beam.h"

FString UCAnimNotify_SpawnBeam::GetNotifyName_Implementation() const
{
	return "SpawnBeam";
}


void UCAnimNotify_SpawnBeam::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	CheckNull(weapon->GetSkillAction(2));
	UCSkillAction_Wizard_Beam* spawnSkill = Cast<UCSkillAction_Wizard_Beam>(weapon->GetSkillAction(2));
	CheckNull(spawnSkill);

	spawnSkill->SpawnBeam();




}