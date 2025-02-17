// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_SetCinematic.h"
#include "Global.h"
#include "Characters/CEnemy_AI_Boss.h"

FString UCAnimNotify_SetCinematic::GetNotifyName_Implementation() const
{
	return "SetCinematic";
}

void UCAnimNotify_SetCinematic::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy_AI_Boss* Boss = Cast<ACEnemy_AI_Boss>(MeshComp->GetOwner());
	CheckNull(Boss);

	Boss->SetCinematic(bCinematic);
	CheckTrue(bCinematic);

	
}
