// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_Slowmotion.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CSkillAction.h"

FString UCAnimNotify_Slowmotion::GetNotifyName_Implementation() const
{
	return "SlowMotion_Parry";
}

void UCAnimNotify_Slowmotion::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetSkillAction(SkillNum));


	weapon->GetSkillAction(SkillNum)->SlowTime();
}