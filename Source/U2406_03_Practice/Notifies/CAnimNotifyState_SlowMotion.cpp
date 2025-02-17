// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_SlowMotion.h"
#include "Global.h"
#include "GameFramework/Character.h"

FString UCAnimNotifyState_SlowMotion::GetNotifyName_Implementation() const
{
	return "SlowMotion";
}

void UCAnimNotifyState_SlowMotion::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	
	UWorld* world = MeshComp->GetOwner()->GetWorld();
	CheckNull(world);

	ULevel* level = world->GetCurrentLevel();
	CheckNull(level);


	TArray<AActor*> actors = level->Actors;
	CheckTrue(actors.Num() < 1);

	for (AActor* actor : actors)
	{
		ACharacter* character = Cast<ACharacter>(actor);
		if (!!character)
		{
			//¸ØÃçÁØ´Ù.
			character->CustomTimeDilation = Timedeltaion;
			characters.Add(character);
		}

		else
			continue;

	}
}

void UCAnimNotifyState_SlowMotion::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	Super::NotifyEnd(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	CheckTrue(characters.Num() <1 );

	for (ACharacter* characater : characters)
	{

		if (!!characater)
			characater->CustomTimeDilation = 1.0f;
		else
			continue;

	}

	characters.Empty();
}