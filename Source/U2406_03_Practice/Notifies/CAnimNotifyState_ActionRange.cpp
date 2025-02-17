
// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_ActionRange.h"
#include "Global.h"
#include "GameFramework/Character.h"

FString UCAnimNotifyState_ActionRange::GetNotifyName_Implementation() const
{
	return "ActionRange";
}

void UCAnimNotifyState_ActionRange::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* owner = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(owner);

	TArray<AActor*> Toignores;
	Toignores.Add(owner);

	FTransform SocketTrans = owner->GetMesh()->GetSocketTransform(SocketName);
	
	FVector Start = SocketTrans.GetLocation();
	FVector End = Start + FVector(0, 0, (-1 * Distance));


	//LineTrace
	UKismetSystemLibrary::LineTraceSingle(owner->GetWorld(),
		Start, 
		End,
		ETraceTypeQuery::TraceTypeQuery4,
		false, 
		Toignores,
		EDrawDebugTrace::None, 
		hitResult, true);
}

void UCAnimNotifyState_ActionRange::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	FActorSpawnParameters spawnParams;

	if (hitResult.bBlockingHit)
	{
		UWorld* world = MeshComp->GetOwner()->GetWorld();
		
		CheckNull(world);
		CheckNull(ToSpawnActor);

		world->SpawnActor<AActor>(
			ToSpawnActor,
			hitResult.Location,
			MeshComp->GetOwner()->GetActorRotation(),
			spawnParams
			);
	}
}
