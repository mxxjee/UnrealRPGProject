// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_SpawnArrowToEnemy.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CSkillAction.h"
#include "Weapons/SkillActions/Bow/CSkillAction_Bow_Special.h"

FString UCAnimNotify_SpawnArrowToEnemy::GetNotifyName_Implementation() const
{
	return "SpawnArrowToEnemy";
}

void UCAnimNotify_SpawnArrowToEnemy::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetSkillAction(3));



	UCSkillAction_Bow_Special* SpecialBow = Cast<UCSkillAction_Bow_Special>(weapon->GetSkillAction(3));
	CheckNull(SpecialBow);

	SpecialBow->SpawnArrowToEnemy();

	


}
