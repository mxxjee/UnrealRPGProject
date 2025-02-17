// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_MoveBackward.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CAIController.h"


FString UCAnimNotify_MoveBackward::GetNotifyName_Implementation() const
{
	return "MoveBackward";
}

void UCAnimNotify_MoveBackward::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* owner = Cast<ACharacter>(MeshComp->GetOwner());

	CheckNull(owner);

	ACAIController* controller = Cast<ACAIController>(owner->GetController());
	if (!!controller)
		controller->StopMovement();

	FVector Forward = owner->GetActorForwardVector();
	owner->LaunchCharacter(-Forward * Force,false,false);


}