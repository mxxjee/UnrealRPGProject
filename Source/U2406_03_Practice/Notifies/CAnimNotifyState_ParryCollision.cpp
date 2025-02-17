// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_ParryCollision.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CSkillAction.h"

FString UCAnimNotifyState_ParryCollision::GetNotifyName_Implementation() const
{
	return "ParryCollision";
}

void UCAnimNotifyState_ParryCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());



	
}

void UCAnimNotifyState_ParryCollision::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetSkillAction(SkillNum));

	weapon->GetSkillAction(SkillNum)->OnBoxTrace();
}


void UCAnimNotifyState_ParryCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	Super::NotifyEnd(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());



}