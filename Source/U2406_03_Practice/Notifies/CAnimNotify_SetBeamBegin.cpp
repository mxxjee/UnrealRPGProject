// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_SetBeamBegin.h"
#include "Global.h"
#include "Characters/CEnemy_AI_Boss.h"



FString UCAnimNotify_SetBeamBegin::GetNotifyName_Implementation() const
{
	return "SetBeamBegin";
}

void UCAnimNotify_SetBeamBegin::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy_AI_Boss* boss = Cast<ACEnemy_AI_Boss>(MeshComp->GetOwner());
	CheckNull(boss);

	boss->SetbBeamBegin(bBeamBegin);
}
