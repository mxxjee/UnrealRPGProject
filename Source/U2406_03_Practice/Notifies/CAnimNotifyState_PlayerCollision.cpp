// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_PlayerCollision.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Components/CWeaponComponent.h"
#include "Characters/CPlayer.h"

FString UCAnimNotifyState_PlayerCollision::GetNotifyName_Implementation() const
{
	return "Player_Collision";
}

void UCAnimNotifyState_PlayerCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->OffCollision();
}

void UCAnimNotifyState_PlayerCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	Super::NotifyEnd(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->OnCollision();
}
