// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_CollisionEnemy.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"


FString UCAnimNotifyState_CollisionEnemy::GetNotifyName_Implementation() const
{
	return "CollisionEnemy";
}

void UCAnimNotifyState_CollisionEnemy::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(MeshComp->GetOwner());
	CheckNull(enemy);

	enemy->OffCollision();
}

void UCAnimNotifyState_CollisionEnemy::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	Super::NotifyEnd(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy_AI* enemy = Cast<ACEnemy_AI>(MeshComp->GetOwner());
	CheckNull(enemy);

	enemy->OnCollision();
}