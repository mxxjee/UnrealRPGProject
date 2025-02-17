// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_EndPakour.h"
#include "Global.h"
#include "Components/CParkourComponent.h"

FString UCAnimNotify_EndPakour::GetNotifyName_Implementation() const
{
	return "EndParkour";
}

void UCAnimNotify_EndPakour::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCParkourComponent* parkour = CHelpers::GetComponent<UCParkourComponent>(MeshComp->GetOwner());
	CheckNull(parkour);

	parkour->End_DoParkour();
}
