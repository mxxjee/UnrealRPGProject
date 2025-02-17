// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/State/CAnimNotify_EndAwake.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CAttackPointComponent.h"
#include "Weapons/CDoAction.h"

FString UCAnimNotify_EndAwake::GetNotifyName_Implementation() const
{
	return "EndAwake";
}

void UCAnimNotify_EndAwake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	UCAttackPointComponent* attack = CHelpers::GetComponent<UCAttackPointComponent>(MeshComp->GetOwner());
	CheckNull(attack);
	
	attack->End_AwakeMontage();
}
