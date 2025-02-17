// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_SpawnActor.h"
#include "Global.h"


FString UCAnimNotify_SpawnActor::GetNotifyName_Implementation() const
{
	return "SpawnActor";
}

void UCAnimNotify_SpawnActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp->GetOwner());
	CheckNull(ToSpawnActor);

	FActorSpawnParameters spawnParams;

	UWorld* world = MeshComp->GetOwner()->GetWorld();

	CheckNull(world);
	world->SpawnActor<AActor>(ToSpawnActor, MeshComp->GetOwner()->GetActorLocation() + OffSet,
		MeshComp->GetOwner()->GetActorRotation() + RotateOffSet, spawnParams);
		
		
	

}

