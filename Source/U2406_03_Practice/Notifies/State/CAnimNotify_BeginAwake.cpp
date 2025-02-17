// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/State/CAnimNotify_BeginAwake.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CAttackPointComponent.h"
#include "Weapons/CDoAction.h"


FString UCAnimNotify_BeginAwake::GetNotifyName_Implementation() const
{
	return "BeginAwake";
}

void UCAnimNotify_BeginAwake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->ExplosionPostProcessSetting();
	UCAttackPointComponent* attack = CHelpers::GetComponent<UCAttackPointComponent>(MeshComp->GetOwner());
	CheckNull(attack);

	attack->Begin_AwakeMonatge();
}

