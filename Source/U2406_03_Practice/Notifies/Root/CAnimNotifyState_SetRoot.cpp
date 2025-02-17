// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/Root/CAnimNotifyState_SetRoot.h"
#include "Global.h"
#include "GameFramework/Character.h"

FString UCAnimNotifyState_SetRoot::GetNotifyName_Implementation() const
{
	return "SetRoot";
}

void UCAnimNotifyState_SetRoot::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	Owner = Cast<ACharacter>(MeshComp->GetOwner());

}

void UCAnimNotifyState_SetRoot::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	CheckNull(Owner);
	

	Owner->SetActorLocation(MeshComp->GetBoneLocation("root", EBoneSpaces::WorldSpace) + FVector(0, 0, 88.f));

}

void UCAnimNotifyState_SetRoot::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation);

	CheckNull(Owner);


	Owner->SetActorLocation(MeshComp->GetBoneLocation("root", EBoneSpaces::WorldSpace)+ FVector(0, 0, 88.f));


}
