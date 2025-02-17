// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_EndGroundDash.h"
#include "Global.h"
#include "Components/CMovementComponent.h"

FString UCAnimNotify_EndGroundDash::GetNotifyName_Implementation() const
{
	return "EndGroundDash";
}

void UCAnimNotify_EndGroundDash::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCMovementComponent* Movement = CHelpers::GetComponent<UCMovementComponent>(MeshComp->GetOwner());
	CheckNull(Movement);

	Movement->OnGroundDash_End();

	Movement->SetbGroundDash(false); 



}
