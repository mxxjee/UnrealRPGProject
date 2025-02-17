// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_Move.h"
#include "Global.h"
#include "Components/CMovementComponent.h"

FString UCAnimNotify_Move::GetNotifyName_Implementation() const
{
	return "Move";
}

void UCAnimNotify_Move::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCMovementComponent* Movement = CHelpers::GetComponent<UCMovementComponent>(MeshComp->GetOwner());
	CheckNull(Movement);

	Movement->MoveCharacter(Force);

	CheckFalse(bEnd);

	MeshComp->GetOwner()->SetActorLocation(MeshComp->GetBoneLocation("root",EBoneSpaces::WorldSpace)+FVector(0,0,88.f));


}
