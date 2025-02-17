
// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_EndAirDash.h"
#include "Global.h"
#include "Components/CMovementComponent.h"
#include "Characters/CPlayer.h"

FString UCAnimNotify_EndAirDash::GetNotifyName_Implementation() const
{
	return "EndAirDash";
}

void UCAnimNotify_EndAirDash::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


	
	UCMovementComponent* Movement = CHelpers::GetComponent<UCMovementComponent>(MeshComp->GetOwner());
	CheckNull(Movement);

	Movement->OnAirDash_End();

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	player->SetIsJump(false);
	
	
}

