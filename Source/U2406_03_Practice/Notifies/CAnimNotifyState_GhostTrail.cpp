// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_GhostTrail.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/DoActions/CDoAction_Combo.h"

FString UCAnimNotifyState_GhostTrail::GetNotifyName_Implementation() const
{
	return "Destroy_GhostTrail";
}


void UCAnimNotifyState_GhostTrail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}


void UCAnimNotifyState_GhostTrail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());

	UCDoAction_Combo* combo = Cast<UCDoAction_Combo>(weapon->GetDoAction());
	CheckNull(combo);

	combo->DestroyGhostTrail();
}
