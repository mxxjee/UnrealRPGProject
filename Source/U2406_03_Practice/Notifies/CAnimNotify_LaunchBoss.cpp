// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_LaunchBoss.h"
#include "Global.h"
#include "Characters/CEnemy_AI_Boss.h"

FString UCAnimNotify_LaunchBoss::GetNotifyName_Implementation() const
{
	return "LaunchBoss";
}

void UCAnimNotify_LaunchBoss::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACEnemy_AI_Boss* Boss = Cast<ACEnemy_AI_Boss>(MeshComp->GetOwner());
	CheckNull(Boss);

	if (bStopLaunch)
	{
		Boss->StopLaunchBoss();
		return;
	}

	else
		Boss->LaunchBoss();
}
