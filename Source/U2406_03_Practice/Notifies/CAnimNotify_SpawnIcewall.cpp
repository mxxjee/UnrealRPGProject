// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_SpawnIcewall.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CSkillAction.h"
#include "Weapons/SkillActions/CSkillAction_Wizard_Wall.h"


FString UCAnimNotify_SpawnIcewall::GetNotifyName_Implementation() const
{
	return "SpawnIcewall";
}


void UCAnimNotify_SpawnIcewall::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	CheckNull(weapon->GetSkillAction(3));
	UCSkillAction_Wizard_Wall* spawnSkill = Cast<UCSkillAction_Wizard_Wall>(weapon->GetSkillAction(3));
	CheckNull(spawnSkill);

	spawnSkill->SpawnIceWalls();




}
