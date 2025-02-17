// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_CameraChange.h"
#include "Global.h"
#include "Characters/CPlayer.h"

FString UCAnimNotify_CameraChange::GetNotifyName_Implementation() const
{
	return "CameraChange";
}

void UCAnimNotify_CameraChange::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	
	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);


	player->AircomboPlayCameraAnimation();
}