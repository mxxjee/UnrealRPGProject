// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_RideHorse.h"
#include "Global.h"
#include "Characters/CPlayer.h"


FString UCAnimNotify_RideHorse::GetNotifyName_Implementation() const
{
	return "RideHorse";
}

void UCAnimNotify_RideHorse::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp->GetOwner());
	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());

	CheckNull(player);

	player->SpawnHorse();



}
